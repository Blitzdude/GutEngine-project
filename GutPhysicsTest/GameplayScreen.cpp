#include "GameplayScreen.h"
#include <iostream>
#include <SDL/SDL.h>
#include <Gutengine/IMainGame.h>
#include <Gutengine/ResourceManager.h>
#include <random>
#include <time.h>
#include "ScreenIndices.h"

const float DELTA_TIME = 1.0f / 60.0f; // TODO: There is no deltatiming in gutengine. Fix?
const int NUM_RIGID_BOXES = 1;

GameplayScreen::GameplayScreen(Gutengine::Window* window) : m_window(window){
    m_screenIndex = SCREEN_INDEX_GAMEPLAY;
}

GameplayScreen::~GameplayScreen() {

}

int GameplayScreen::getNextScreenIndex() const {
    return SCREEN_INDEX_NO_SCREEN;
}

int GameplayScreen::getPreviousScreenIndex() const {
    return SCREEN_INDEX_NO_SCREEN;
}

void GameplayScreen::build() {
}

void GameplayScreen::destroy() {
}

void GameplayScreen::onEntry() {

    // Load the texture
	Gutengine::ResourceManager::getTexture("assets/blank.png");
    // Initialize spritebatch
    m_spriteBatch.init();

    // Shader init
    // Compile our texture shader
    m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    m_textureProgram.addAttribute("vertexPosition");
    m_textureProgram.addAttribute("vertexColor");
    m_textureProgram.addAttribute("vertexUV");
    m_textureProgram.linkShaders();
    // Compile our light shader
    m_lightProgram.compileShaders("Shaders/lightShading.vert", "Shaders/lightShading.frag");
    m_lightProgram.addAttribute("vertexPosition");
    m_lightProgram.addAttribute("vertexColor");
    m_lightProgram.addAttribute("vertexUV");
    m_lightProgram.linkShaders();

	// init debug renderer
	m_debugRenderer.init();

    // Init camera
    m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
    m_camera.setScale(1.0f);

	// Init GutPhysics gravity
	//m_physicsSystem.setGravity({ 0.0f, -0.02f });

	// Init random generator
	std::mt19937 randGenerator;
	randGenerator.seed((unsigned int)time(NULL));

	std::uniform_real<float> _xPos(-10.0, 800.0f);
	std::uniform_real<float> _yPos(-10.0, 500.0f);
	std::uniform_real<float> _w(10.0, 50.0f);
	std::uniform_real<float> _h(10.0, 50.0f);

	// Init rigidBodies
	for (int i = 0; i < NUM_RIGID_BOXES; ++i) 
	{
		glm::vec2 pos(_xPos(randGenerator), _yPos(randGenerator));
		m_rects.push_back(Gutengine::Rectangle(glm::vec2(100, 100), 200, 100));
		/*
		
		m_physicsSystem.getRigidbodyList().push_back( new Gutengine::RigidBody2D(
				m_camera.convertScreenToWorld(pos),
				glm::vec2(0.0f, 0.0f),
				_w(randGenerator),
				_h(randGenerator)
			)
		);
		*/
	}

    initUI();
}

void GameplayScreen::onExit() {
    m_debugRenderer.dispose();
}

void GameplayScreen::update() {
	//std::cout << m_game->getFps() << std::endl;
	//m_physicsSystem.updatePhysics();

    m_camera.update();
    checkInput();
	for (auto &r : m_rects)
	{
		r.accelerationAng = -r.velocityAng * 0.4f;
		r.Update(DELTA_TIME);
	}
}

void GameplayScreen::draw() {
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

    m_spriteBatch.begin();
	// Draw code goes here
	glm::vec4 destRect;
	glm::vec4 uvRect = { 0.0f, 1.0f, 0.0f, 1.0f };

    m_spriteBatch.end();

    m_spriteBatch.renderBatch();
    m_textureProgram.unuse();

    // Debug rendering
    if (m_renderDebug) {
		for (auto& itr : m_rects) {
			itr.DebugDraw(m_debugRenderer);
			glm::vec4 ab;
			ab.x = itr.GetAABB().pos.x;
			ab.y = itr.GetAABB().pos.y;
			ab.z = itr.GetAABB().w;
			ab.w = itr.GetAABB().h;
			m_debugRenderer.drawBox(ab, Gutengine::ColorRGBA8(255, 255, 255, 255), 0.0f);
		}
		// Render
        m_debugRenderer.end();
        m_debugRenderer.render(projectionMatrix, 2.0f);
    }

    m_gui.draw();
}

void GameplayScreen::initUI() {
    // Init the UI
    m_gui.init("GUI");
    m_gui.loadScheme("TaharezLook.scheme");
    m_gui.setFont("DejaVuSans-10");
	// Init button object
    CEGUI::PushButton* testButton = static_cast<CEGUI::PushButton*>(m_gui.createWidget("TaharezLook/Button", glm::vec4(0.01f, 0.01f, 0.1f, 0.05f), glm::vec4(0.0f), "TestButton"));
    testButton->setText("Exit Game!");

	// init fps counter TODO: Not used
	CEGUI::Window* fpsText = static_cast<CEGUI::Window*>(m_gui.createWidget("TaharezLook/Label", glm::vec4(0.1f, 0.01f, 0.1f, 0.05f), glm::vec4(0.0f), "fpsText"));

    // Set the event to be called when we click
    testButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameplayScreen::onExitClicked, this));

    //m_gui.setMouseCursor("TaharezLook/MouseArrow");
    m_gui.showMouseCursor();
    SDL_ShowCursor(1);
}


void GameplayScreen::checkInput() {
    SDL_Event evnt;
    while (SDL_PollEvent(&evnt)) {
        m_game->onSDLEvent(evnt);
        m_gui.onSDLEvent(evnt);
        switch (evnt.type) {
            case SDL_QUIT:
                onExitClicked(CEGUI::EventArgs());
                break;
        }
    }
	// LMB or RMB down
	if (m_game->inputManager.isKeyPressed(SDL_BUTTON_LEFT) || m_game->inputManager.isKeyPressed(SDL_BUTTON_RIGHT))
	{
		glm::vec2 mouse = m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords());
		for (auto &s : m_rects)
		{
			if (s.GetAABB().isPointIn(mouse))
			{
				std::cout << "Shape selected!" << std::endl;
				m_selectedShape = &s;
			}
		}
	}

	// LMB hold
	if (m_game->inputManager.isKeyDown(SDL_BUTTON_LEFT))
	{
		if (m_selectedShape != nullptr)
		{
			glm::vec2 offset = glm::vec2(m_selectedShape->width / 2.0f, m_selectedShape->height / 2.0f);
			glm::vec2 mouse = m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords() );
			// Set new position for selected shape
			m_selectedShape->position = mouse - offset;
		}
	}

	// LMB UP
	if (m_game->inputManager.isKeyReleased(SDL_BUTTON_LEFT))
	{
		m_selectedShape = nullptr;
	}
	// RMB UP
	if (m_game->inputManager.isKeyReleased(SDL_BUTTON_RIGHT))
	{
		if (m_selectedShape != nullptr)
		{
			m_selectedShape->velocityAng += 10.0f;
			m_selectedShape = nullptr;
		}
	}
}


bool GameplayScreen::onExitClicked(const CEGUI::EventArgs& e) {
    m_currentState = Gutengine::ScreenState::EXIT_APPLICATION;
    return true;
}