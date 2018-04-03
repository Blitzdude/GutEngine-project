#include "GameplayScreen.h"
#include <iostream>
#include <SDL/SDL.h>
#include <Gutengine/IMainGame.h>
#include <Gutengine/ResourceManager.h>
#include <random>

#include "ScreenIndices.h"

//#include "Light.h"

// GLOBALS
const int CELL_SIZE = 24;
const float FORCE_MAX = 24.0f;

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
	
	// Initialize grid
	m_grid = std::make_unique<Grid>(m_window->getScreenWidth(), m_window->getScreenHeight(), CELL_SIZE);

	// Initialize debug renderer
    m_debugRenderer.init();

    // Initialize spritebatch
    m_spriteBatch.init();

	// Initialize inputmanager
    // Shader init
    // Compile our texture program
    m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    m_textureProgram.addAttribute("vertexPosition");
    m_textureProgram.addAttribute("vertexColor");
    m_textureProgram.addAttribute("vertexUV");
    m_textureProgram.linkShaders();

    // Init camera

    m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	// move the camera, so the world origin is in the bottom-left corner
	m_camera.setPosition(glm::vec2(m_window->getScreenWidth() / 2.0f, m_window->getScreenHeight() / 2.0f));

	// Init cell texture
	m_grid->m_cellTexture = Gutengine::ResourceManager::getTexture("Assets/blank.png");

	// initialize cell colors FOR FUN
	/*
	for (int j = 0; j < m_grid->getNumYCells(); j++)
		for (int i = 0; i < m_grid->getNumXCells(); i++)
			m_grid->getCell(i, j)->color = Gutengine::ColorRGBA8(i % 256, j % 256, (i*j) % 256, 255);
	*/

    initUI();
}

void
GameplayScreen::onExit() {
	m_textureProgram.dispose();
    m_debugRenderer.dispose();
    m_world.reset();
}

void
GameplayScreen::update() {
    m_camera.update();
	m_grid->update(m_game->inputManager, m_camera);
    checkInput();

    // Update the physics simulation DELETE
    //m_world->Step(1.0f / 60.0f, 6, 2);
}

void 
GameplayScreen::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_textureProgram.use();

    // Upload texture uniform
    GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);
    glActiveTexture(GL_TEXTURE0);

    // Camera matrix
    glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
    GLint pUniform = m_textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	// begin spritebatch drawing
    m_spriteBatch.begin();
	
	glm::vec4 destRect;

	// draw the grid squares by color
	for (int j = 0; j < m_grid->getNumYCells(); j++) {
		for (int i = 0; i < m_grid->getNumXCells(); i++) {
			auto curCell = m_grid->getCell(i, j);
			glm::vec2 currentPos  = m_grid->getCellPos(i, j);


			m_spriteBatch.draw(
				glm::vec4(currentPos.x, currentPos.y, CELL_SIZE, CELL_SIZE),
				glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
				m_grid->m_cellTexture.id, 0.0f,
				curCell->color);
		}
	}
	
	m_spriteBatch.end();
	// add grid rendering here
    m_spriteBatch.renderBatch();
    m_textureProgram.unuse();

    // Debug rendering
    if (m_renderDebug) {
        glm::vec4 destRect;
		////////////////////////////////
		// Draws a gray grid for cells
		for (int y = 0; y <= m_grid->getNumYCells(); y++) { // y direction
			glm::vec2 start = glm::vec2(0.0f, (float)y*CELL_SIZE);
			glm::vec2 end = glm::vec2(m_grid->getNumXCells()*CELL_SIZE, (float)y*CELL_SIZE);
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
		if (m_mouse1) {
			
			// make sure the vectors size is greater then one
			if (m_mouseCoordVector.size() > 1) {

				for (auto itr = m_mouseCoordVector.begin();
					itr != m_mouseCoordVector.end() - 1;	// last element
					++itr) 
				{
					m_debugRenderer.drawLine(*itr, *std::next(itr), 
						Gutengine::ColorRGBA8(0, 255, 0, 255));
				}
			}
			else if (m_mouseCoordVector.empty()) {
				// if the mouseCoordsVector is empty, push coords to it
				m_mouseCoordVector.push_back(
					m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords()));
			}

			// if mouse is adjequet distance away from last saved point, push it to the vector
			if (glm::distance(m_mouseCoordVector.back(), m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords())) > CELL_SIZE)
			{
				glm::vec2 back = m_mouseCoordVector.back();
				glm::vec2 temp =  m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords()) - back;
				
				m_mouseCoordVector.push_back(m_mouseCoordVector.back() + glm::normalize(temp) * 24.0f);
			}

			// FUN: Remove front elements until size ok
			while (m_mouseCoordVector.size() >= 10 )
			{
				m_mouseCoordVector.erase(m_mouseCoordVector.begin());
			}

			// draw the mouseline green
			m_debugRenderer.drawLine(
				m_mouseCoordVector.back(),
				m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords()),
				Gutengine::ColorRGBA8(0, 255, 0, 255));
			// draw the dots for vector elements
			for (auto itr : m_mouseCoordVector) {
				m_debugRenderer.drawCircle(
					itr, Gutengine::ColorRGBA8(255, 125, 125, 255),
					20.0f, 40 // FUN: size is big
				);
			}
		}
		else {
			if (!m_mouseCoordVector.empty()) {
				m_isForceDirty = true;
			}
		}

		if (m_isForceDirty) { // if force cells need to be updated
			// DEBUG : print the vector
			for (auto itr = m_mouseCoordVector.begin(); std::next(itr) != m_mouseCoordVector.end(); ++itr) {
				std::cout << "current: " << itr->x << " " << itr->y
					<< " next: " << std::next(itr)->x << " " << std::next(itr)->y << std::endl;
			}
			std::cout << std::endl;
			for (auto itr = m_mouseCoordVector.begin(); std::next(itr) != m_mouseCoordVector.end(); ++itr) {
				
				// if the cells force is zero
				if (m_grid->getCell(*itr)->force == glm::vec2(0.0f, 0.0f)) {
					Cell* cell = m_grid->getCell(*itr);

					glm::vec2 temp = glm::normalize(m_grid->getCellPos(*std::next(itr) - *itr));
					cell->setForce(temp * FORCE_MAX);
					cell->color = Gutengine::ColorRGBA8(
						atan2f(cell->force.y, cell->force.x) * 255,
						cell->force.y,
						cell->force.x,
						255);
					
				}
			}
			// clear the vector
			m_mouseCoordVector.clear();
			// set the flag to not dirty
			m_isForceDirty = false;
		}

		m_debugRenderer.end();
        m_debugRenderer.render(projectionMatrix, 2.0f);
    }
   
    m_gui.draw();
}

void
GameplayScreen::initUI() {
    // Init the UI
    m_gui.init("GUI");
    m_gui.loadScheme("TaharezLook.scheme");
    m_gui.setFont("DejaVuSans-10");
	// Exit button
    CEGUI::PushButton* testButton = static_cast<CEGUI::PushButton*>(m_gui.createWidget("TaharezLook/Button", glm::vec4(0.01f, 0.01f, 0.1f, 0.05f), glm::vec4(0.0f), "TestButton"));
    testButton->setText("Exit Game!");
	
    // Set the event to be called when we click
    testButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameplayScreen::onExitClicked, this));

    SDL_ShowCursor(1);
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
	if (m_game->inputManager.isKeyDown(SDLK_UP))
		m_camera.offsetPosition(glm::vec2(0.0f, 1.0f));

	else if (m_game->inputManager.isKeyDown(SDLK_DOWN))
		m_camera.offsetPosition(glm::vec2(0.0f, -1.0f));

	if (m_game->inputManager.isKeyDown(SDLK_LEFT))
		m_camera.offsetPosition(glm::vec2(-1.0f, 0.0f));

	else if (m_game->inputManager.isKeyDown(SDLK_RIGHT))
		m_camera.offsetPosition(glm::vec2(1.0f, 0.0f));

	if (m_game->inputManager.isKeyDown(SDLK_ESCAPE))
		GameplayScreen::onExitClicked(CEGUI::EventArgs());

	if (m_game->inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
		m_mouse1 = true;
	}
	else {
		m_mouse1 = false;
	}

	
}

bool
GameplayScreen::onExitClicked(const CEGUI::EventArgs& e) {
    m_currentState = Gutengine::ScreenState::EXIT_APPLICATION;
    return true;
}