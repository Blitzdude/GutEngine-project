#include "GameplayScreen.h"
#include <iostream>
#include <SDL/SDL.h>
#include <Gutengine/IMainGame.h>
#include <Gutengine/ResourceManager.h>
#include <random>
#include <list>

#include "ScreenIndices.h"

//#include "Light.h"

// GLOBALS
const int CELL_SIZE = 32;
const int CELL_SIZE_HALF = CELL_SIZE / 2;
const int MAP_HEIGHT = 2048;
const int MAP_WIDTH = 1656;
const float OBJECT_SIZE = 24.0f;

GameplayScreen::GameplayScreen(Gutengine::Window* window) : m_window(window){
    m_screenIndex = SCREEN_INDEX_GAMEPLAY;
}

GameplayScreen::~GameplayScreen() {

}

int
GameplayScreen::getNextScreenIndex() const {
    return SCREEN_INDEX_NO_SCREEN;
}

int
GameplayScreen::getPreviousScreenIndex() const {
    return SCREEN_INDEX_MAINMENU;
}

void
GameplayScreen::build() {
	// empty
}

void
GameplayScreen::destroy() {
	// Empty
}

void
GameplayScreen::onEntry() {

	// create box2D world
    b2Vec2 gravity(0.0f, 0.0f);
    m_world = std::make_unique<b2World>(gravity);
    // create world boundary
    createBoundary();
	
	// Initialize grid
	m_grid = std::make_unique<Grid>(MAP_WIDTH, MAP_HEIGHT, CELL_SIZE);
    
   

	// Initialize debug renderer
    m_debugRenderer.init();

    // Initialize spritebatches
    m_spriteBatch.init();
    m_spriteFont.init("Fonts/chintzy.ttf", 24);
	// Initialize inputmanager

    // Shader init
    // Compile our texture program
    m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    m_textureProgram.addAttribute("vertexPosition");
    m_textureProgram.addAttribute("vertexColor");
    m_textureProgram.addAttribute("vertexUV");
    m_textureProgram.linkShaders();

    // Init cameras
    m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());

	// move the camera, so the world origin is in the bottom-left corner
	m_camera.setPosition(glm::vec2(m_window->getScreenWidth() / 2.0f, m_window->getScreenHeight() / 2.0f));
	// Set camera scale 
	m_camera.setScale(1.0f);
    // Ui Camera
    m_uiCamera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
    m_uiCamera.setScale(1.0f);
    
    // Init backgroud texture
    m_background = Gutengine::ResourceManager::getTexture("Assets/map_paper_small.png");


	// init objects
	const int NUMBER_OF_OBJECTS = 4;

	for (int i = 0; i < NUMBER_OF_OBJECTS; i++) {
		Circle obj;
        obj.init(m_world.get(), glm::vec2(300.0f + i * 40.f, 300.0f), OBJECT_SIZE, OBJECT_SIZE, Gutengine::ResourceManager::getTexture("Assets/circleBordered.png"),
            Gutengine::ColorRGBA8(0, 0, 255, 255), true, b2_dynamicBody);
        obj.setTeam(m_currentTeam);
        
		m_objects.push_back(obj);
	}
    for (auto itr : m_objects)
        

    initUI();
}

void
GameplayScreen::onExit() {
	m_textureProgram.dispose();
    m_spriteFont.dispose();
    m_debugRenderer.dispose();
    m_world.reset();
}

void
GameplayScreen::update() {
    m_camera.update();
    m_uiCamera.update();
	m_grid->update(m_game->inputManager, m_camera);
    checkInput();
	updateMouse();
	// update objects
	for (auto itr : m_objects) {
		itr.update(*m_grid);
	}
    // Update the physics simulation
    m_world->Step(1.0f / 60.0f, 6, 2);
}

void 
GameplayScreen::draw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_textureProgram.use();

    // Camera matrix
    glm::mat4 projectionMatrix = m_camera.getCameraMatrix();

    // Upload texture uniform
    GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);
    glActiveTexture(GL_TEXTURE0);

    GLint pUniform = m_textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    m_spriteBatch.begin();

    //Draw background
    
    m_spriteBatch.draw(glm::vec4(0.0f, 0.0f, m_background.width, m_background.height),
        glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
        m_background.id,
        0.0f,
        Gutengine::ColorRGBA8(255, 255, 255, 255));
    
    
    // Draw objects
	for (auto itr : m_objects) {
		itr.draw(m_spriteBatch);
	}

	m_spriteBatch.end();
    m_spriteBatch.renderBatch();

    m_textureProgram.unuse();

    drawDebug(projectionMatrix);

    drawUI();

    m_gui.draw();
}

void
GameplayScreen::initUI() {
    // Init the UI
    m_gui.init("GUI");
    m_gui.loadScheme("TaharezLook.scheme");
    m_gui.setFont("DejaVuSans-10");

	// Exit button
    CEGUI::PushButton* exitButton = static_cast<CEGUI::PushButton*>(m_gui.createWidget("TaharezLook/Button", glm::vec4(0.01f, 0.01f, 0.1f, 0.05f), glm::vec4(0.0f), "exitButton"));
    exitButton->setText("Exit Game!");
	
    // Team Toggle button
    CEGUI::PushButton* teamToggleButton = static_cast<CEGUI::PushButton*>(m_gui.createWidget("TaharezLook/Button", glm::vec4(0.12f, 0.01f, 0.1f, 0.05f), glm::vec4(0.0f), "teamToggleButton"));
    teamToggleButton->setText("Toggle Team!");



    // Set the event to be called when we click
    exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameplayScreen::onExitClicked, this));
    teamToggleButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameplayScreen::onTeamToggleClicked, this));


    SDL_ShowCursor(1);
}

void GameplayScreen::createBoundary()
{
    makeEdge( MAP_WIDTH / 2.0f, MAP_HEIGHT, MAP_WIDTH, 10.0f); // top edge
    makeEdge( MAP_WIDTH / 2.0f, 0.0f, MAP_WIDTH, 10.0f); // bottom edge
    makeEdge( 0.0f,       MAP_HEIGHT / 2.0f, 10.0f, MAP_HEIGHT); // left edge
    makeEdge( MAP_WIDTH,  MAP_HEIGHT / 2.0f, 10.0f, MAP_HEIGHT); // right edge

}

void GameplayScreen::makeEdge(float x, float y, float w, float h) {
    // make the level edges
    b2BodyDef BarrierDef;
    BarrierDef.position.Set(x, y);

    //allocates barrier in memory
    b2Body* Barrier = m_world->CreateBody(&BarrierDef);
    // make the ground fixture - bind the shape of the body.
    b2PolygonShape box;
    box.SetAsBox(w, h);
    Barrier->CreateFixture(&box, 0.0f); // density is 0.0f so they can't be moved
    //make level edges - end
}

void GameplayScreen::drawDebug(glm::mat4 projMat)
{

    // Debug rendering
    if (m_renderDebug)
    {
        ////////////////////////////////
        // Draws a gray grid for cells
        for (int y = 0; y <= m_grid->getNumYCells(); y++) { // y direction
            glm::vec2 start = glm::vec2(0.0f, (float)y * CELL_SIZE);
            glm::vec2 end = glm::vec2(m_grid->getNumXCells()*CELL_SIZE, (float)y * CELL_SIZE);
            if (y % 10 == 0)
                m_debugRenderer.drawLine(start, end, Gutengine::ColorRGBA8(255, 255, 255, 100));
            else
                m_debugRenderer.drawLine(start, end, Gutengine::ColorRGBA8(255, 255, 255, 40));

        }
        for (int x = 0; x <= m_grid->getNumXCells(); x++) { // x direction
            glm::vec2 start = glm::vec2((float)x*CELL_SIZE, 0.0f);
            glm::vec2 end = glm::vec2((float)x*CELL_SIZE, m_grid->getNumYCells()*CELL_SIZE);
            if (x % 10 == 0)
                m_debugRenderer.drawLine(start, end, Gutengine::ColorRGBA8(255, 255, 255, 100));
            else
                m_debugRenderer.drawLine(start, end, Gutengine::ColorRGBA8(255, 255, 255, 40));
        }
        /*****************************/
        //////////////////////////////

        // draw a line if mouse1 is down
        if (m_mouse1)
        {
            for (auto itr = m_mouseCoordVector.begin(); std::next(itr) != m_mouseCoordVector.end(); ++itr) {
                m_debugRenderer.drawLine(
                    *itr,
                    *std::next(itr), // next element
                    Gutengine::ColorRGBA8(0, 255, 0, 255));
            }

            // draw the mouseline green

            m_debugRenderer.drawLine(
                m_mouseCoordVector.back(),
                m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords()),
                Gutengine::ColorRGBA8(0, 255, 0, 255));
        }

        for (int j = 0; j < m_grid->getNumYCells(); ++j)
        {
            for (int i = 0; i < m_grid->getNumXCells(); ++i)
            {
                if (m_grid->getCell(i, j)->getForce() != glm::vec2(0.0f))
                    visualizeForce(m_grid->getCellPos(i, j));
            }
        }

        // Draw edges
        // Top
        m_debugRenderer.drawLine({ 0.0f, MAP_HEIGHT }, { MAP_WIDTH, MAP_HEIGHT }, Gutengine::ColorRGBA8(255, 0, 255, 140));
        // bottom
        m_debugRenderer.drawLine({ 0.0f, 0.0f }, { MAP_WIDTH, 0.0f }, Gutengine::ColorRGBA8(255, 0, 255, 140));
        // left
        m_debugRenderer.drawLine({ 0.0f, 0.0f }, { 0.0f, MAP_HEIGHT }, Gutengine::ColorRGBA8(255, 0, 255, 140));
        // right
        m_debugRenderer.drawLine({ MAP_WIDTH, 0.0f }, { MAP_WIDTH, MAP_HEIGHT }, Gutengine::ColorRGBA8(255, 0, 255, 140));

        m_debugRenderer.end();
        m_debugRenderer.render(projMat, 2.0f);
    }

}

void GameplayScreen::drawUI()
{

    m_textureProgram.use();
    

    // Upload texture uniform
    GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);
    glActiveTexture(GL_TEXTURE0);

    glm::mat4 projectionMatrix = m_uiCamera.getCameraMatrix();
    GLint pUniform = m_textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    m_spriteBatch.begin();
    // draw sprite font
    glm::vec2 textPos = m_uiCamera.convertScreenToWorld({ 10.0f, m_window->getScreenHeight() - 10.0f });
    std::string text = "Num Objects: " + std::to_string((int)m_objects.size());
    m_spriteFont.draw(m_spriteBatch, text.c_str(), textPos, glm::vec2(0.7f), 0.0f, Gutengine::ColorRGBA8(255, 255, 255, 255));
    
    m_spriteBatch.end();
    m_spriteBatch.renderBatch();

    m_textureProgram.unuse();
}

void
GameplayScreen::checkInput() {
    SDL_Event evnt;
    while (SDL_PollEvent(&evnt)) 
	{
        m_game->onSDLEvent(evnt);
        m_gui.onSDLEvent(evnt);
        switch (evnt.type) {
            case SDL_QUIT:
                onExitClicked(CEGUI::EventArgs());
                break;
		}
    }
    const float MAX_SCALE = 2.0f;
    const float MIN_SCALE = 0.3f;

	if (m_game->inputManager.isKeyDown(SDLK_w))
		m_camera.offsetPosition(glm::vec2(0.0f, 5.0f));

	else if (m_game->inputManager.isKeyDown(SDLK_s))
		m_camera.offsetPosition(glm::vec2(0.0f, -5.0f));

	if (m_game->inputManager.isKeyDown(SDLK_a))
		m_camera.offsetPosition(glm::vec2(-5.0f, 0.0f));

	else if (m_game->inputManager.isKeyDown(SDLK_d))
		m_camera.offsetPosition(glm::vec2(5.0f, 0.0f));

	if (m_game->inputManager.isKeyDown(SDLK_q)) {

        // scale up till Max
        float s = m_camera.getScale();
        m_camera.setScale((s >= MAX_SCALE) ? s : s + 0.1f);
        std::cout << "Scale: " << m_camera.getScale() << std::endl;
	}

	if (m_game->inputManager.isKeyDown(SDLK_e)) {
        // scale down till min
        float s = m_camera.getScale();
        m_camera.setScale((s < MIN_SCALE) ? s : s - 0.1f);
        std::cout << "Scale: " << m_camera.getScale() << std::endl;
	}

    if (m_game->inputManager.isKeyDown(SDLK_r)) {
        m_grid->resetForces();
    }

	if (m_game->inputManager.isKeyDown(SDLK_ESCAPE))
		GameplayScreen::onExitClicked(CEGUI::EventArgs());

	if (m_game->inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
		m_mouse1 = true;
	}
	else {
		m_mouse1 = false;
	}

    if (m_game->inputManager.isKeyDown(SDL_BUTTON_RIGHT)) {
        m_mouse2 = true;
    }
    else {
        m_mouse2 = false;
    }

}

void 
GameplayScreen::updateMouse() {
	if (m_mouse1) {

		// make sure the vectors size is greater then one

		if (m_mouseCoordVector.empty()) {
			// if the mouseCoordsVector is empty, push mouses current coords to it
			m_mouseCoordVector.push_back(
				m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords()));
		}

		// if mouse is adjequet distance away from last saved point, push it to the vector
		if (glm::distance(m_mouseCoordVector.back(), m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords())) > CELL_SIZE)
		{
			glm::vec2 back = m_mouseCoordVector.back();
			glm::vec2 temp = m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords()) - back;

			m_mouseCoordVector.push_back(m_mouseCoordVector.back() + glm::normalize(temp) * (float)CELL_SIZE);
		}
	}
	else 
	{ // if mouse not down
		if (!m_mouseCoordVector.empty()) 
		{
			m_grid->createFlowField(m_mouseCoordVector,  (1 / ((int)m_camera.getScale() + 1)) + 1 );
            
			m_mouseCoordVector.clear();
		}
	}
    // add objects with second button
    if (m_mouse2)
    {
        glm::vec2 mouseCoords = m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords());
        Circle obj;
        obj.init(m_world.get(), glm::vec2(mouseCoords.x, mouseCoords.y), OBJECT_SIZE, OBJECT_SIZE, Gutengine::ResourceManager::getTexture("Assets/circleBordered.png"),
            Gutengine::ColorRGBA8(0, 0, 255, 255), true, b2_dynamicBody);
        if (m_currentTeam == Team::BLUE)
            obj.setColor(Gutengine::ColorRGBA8(0, 0, 255, 255));
        else if (m_currentTeam == Team::RED)
            obj.setColor(Gutengine::ColorRGBA8(255, 0, 0, 255));

        m_objects.push_back(obj);
    }
}

bool
GameplayScreen::onExitClicked(const CEGUI::EventArgs& e) {
    m_currentState = Gutengine::ScreenState::EXIT_APPLICATION;
    return true;
}

bool GameplayScreen::onTeamToggleClicked(const CEGUI::EventArgs & e)
{
    m_grid->resetForces();
    m_currentTeam = (m_currentTeam == Team::BLUE) ? Team::RED : Team::BLUE;
    return true;
}



void GameplayScreen::visualizeForce(glm::vec2 cellPos)
{
    auto cell = m_grid->getCell(cellPos);
    glm::vec2 cellCenter(cellPos.x + CELL_SIZE_HALF, cellPos.y + CELL_SIZE_HALF);
    m_debugRenderer.drawCircle(cellCenter + cell->getForce() * (float)CELL_SIZE_HALF / 2.0f, Gutengine::ColorRGBA8(255, 255, 255, 140), 4.0f, 3 );

    m_debugRenderer.drawLine(cellCenter - cell->getForce() * ((float)CELL_SIZE_HALF / 2.0f),
                             cellCenter + cell->getForce() * ((float)CELL_SIZE_HALF / 2.0f),
                             Gutengine::ColorRGBA8(255, 255, 255, 140));
}
