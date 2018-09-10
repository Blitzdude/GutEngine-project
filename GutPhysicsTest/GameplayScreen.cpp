#include "GameplayScreen.h"
#include <iostream>
#include <SDL/SDL.h>
#include <Gutengine/IMainGame.h>
#include <Gutengine/ResourceManager.h>
#include <random>
#include <time.h>
#include "ScreenIndices.h"

// const float DELTA_TIME = 1.0f / 60.0f; // TODO: There is no deltatiming in gutengine. Fix?
const int NUM_RIGID_BOXES = 10;

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
	//auto blank = Gutengine::ResourceManager::getTexture("assets/blank.png");
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

	// Init random generator
	std::mt19937 randGenerator;
	randGenerator.seed((unsigned int)time(NULL));

	std::uniform_real<float> _xPos(-200.0, 800.0f);
	std::uniform_real<float> _yPos(-10.0, 300.0f);
	std::uniform_real<float> _w(50.0, 200.0f);
	std::uniform_real<float> _h(50.0, 200.0f);

	m_physicsSystem = std::make_unique<Gutengine::GutPhysics2D>();
	m_physicsSystem->setGravity({0.0f, -40.0f});
	m_physicsSystem->setElasticity(0.20f);

	// Init rigidBodies
	for (int i = 0; i < NUM_RIGID_BOXES; ++i) 
	{
		Gutengine::Rectangle temp(glm::vec2(_xPos(randGenerator), _yPos(randGenerator)) , _w(randGenerator), _h(randGenerator));
		temp.mass = (temp.width * 0.01f) * (temp.height * 0.01f);
		m_physicsSystem->addRigidBody2D(temp);
	}
	// Floor
	Gutengine::Rectangle floor(glm::vec2(0.0f, -400.0f), 1200.0f, 100.0f, 0.0f, 1.0f);
	floor.setStatic(true);
	m_physicsSystem->addRigidBody2D(floor);
	// floating platform
	Gutengine::Rectangle platform(glm::vec2(-200.0f, 0.0f), 600.0f, 100.0f, -0.5f, 1.0f);
	platform.setStatic(true);
	m_physicsSystem->addRigidBody2D(platform);

    initUI();
}

void GameplayScreen::onExit() {
    m_debugRenderer.dispose();
}

void GameplayScreen::update() {
	
	m_physicsSystem->clear();
    m_camera.update();
    checkInput();
	m_physicsSystem->updatePhysics(1 / 60.0f);
	handleWrapping();
	updateUI();
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
		for (auto& itr : m_physicsSystem->getRigidbodyList()) {

			// Draw Rectangle
			glm::vec4 destRect;
			Gutengine::ColorRGBA8 c = { 0,0,0,255 };
			if (itr->getStatic()) {
				c.b = 255; c.g = 255;
			}
			else {
				c.b = 255, c.r = 255;
			}
			destRect.x = itr->position.x;
			destRect.y = itr->position.y;
			destRect.z = itr->width;
			destRect.w = itr->height;
			m_debugRenderer.drawBox(destRect, c, itr->orientation);
			m_debugRenderer.drawCircle(itr->position, Gutengine::ColorRGBA8(255, 0, 0, 255), 2.0f); // Center of mass
			m_debugRenderer.drawLine(itr->position, itr->position + itr->velocity, Gutengine::ColorRGBA8(255, 0, 0, 255));
		}
			
		// RMB hold
		if (m_game->inputManager.isKeyDown(SDL_BUTTON_RIGHT))
		{
			if (!m_selectedShape.expired())
			{
				glm::vec2 mouse = m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords());
				m_debugRenderer.drawLine(mouse, m_torquePoint, Gutengine::ColorRGBA8(255, 0, 0, 255));
			}
		}

		// LMB hold
		if (m_game->inputManager.isKeyDown(SDL_BUTTON_LEFT))
		{
			if (!m_selectedShape.expired())
			{
				glm::vec2 mouse = m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords());
				m_debugRenderer.drawLine(mouse, m_selectedShape.lock()->position, Gutengine::ColorRGBA8(255, 125, 0, 255));
			}
		}

		// manifold drawing
		for (auto itr : m_physicsSystem->getManifolds())
		{
			// Contact point
			m_debugRenderer.drawCircle(itr.contactPoint, Gutengine::ColorRGBA8(0, 0, 255, 255), 5.0f);
			// normal
			m_debugRenderer.drawLine(itr.contactPoint, itr.contactPoint + 50.0f * itr.normal, Gutengine::ColorRGBA8(0, 0, 255, 255));
			// collision edge
			m_debugRenderer.drawLine(itr.contactPoint, itr.contactPoint + 50.0f* glm::normalize(itr.edge.a - itr.edge.b), Gutengine::ColorRGBA8(0, 0, 255, 255));
		}

		// Render
        m_debugRenderer.end();
        m_debugRenderer.render(projectionMatrix, 3.0f);
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
    testButton->setText("Exit Demo!");

	// init fps counter TODO: Not used
	fpsText = static_cast<CEGUI::Window*>(m_gui.createWidget("TaharezLook/Label", glm::vec4(0.1f, 0.01f, 0.1f, 0.05f), glm::vec4(0.0f), "fpsText"));
	fpsText->setText("0.00");

    // Set the event to be called when we click
    testButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameplayScreen::onExitClicked, this));

    //m_gui.setMouseCursor("TaharezLook/MouseArrow");
    m_gui.showMouseCursor();
    SDL_ShowCursor(1);
}

void GameplayScreen::updateUI()
{
	m_gui.update();
	float fps = m_game->getFps();
	fpsText->setText(std::to_string(fps));
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
	// LMB down
	if (m_game->inputManager.isKeyPressed(SDL_BUTTON_LEFT))
	{
		glm::vec2 mouse = m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords());
		for (auto &s : m_physicsSystem->getRigidbodyList())
		{
			if (s->GetAABB().isPointIn(mouse))
			{
				m_selectedShape = s;
				m_torquePoint = mouse;
				break;
			}
		}
	}

	// RMB down
	if (m_game->inputManager.isKeyPressed(SDL_BUTTON_RIGHT))
	{
		glm::vec2 mouse = m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords());
		for (auto &s : m_physicsSystem->getRigidbodyList())
		{
			if (s->GetAABB().isPointIn(mouse))
			{
				m_selectedShape = s;
				m_torquePoint = mouse;
				break;
			}
		}
	}

	// LMB hold -> pull object
	if (m_game->inputManager.isKeyDown(SDL_BUTTON_LEFT))
	{
		if (!m_selectedShape.expired())
		{
			glm::vec2 mouse = m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords() );
			// Set new position for selected shape
			auto s = m_selectedShape.lock();
			if (glm::length(mouse - s->position) < 40.0f)
			{
				s->velocity -= s->velocity * (1.0f / glm::length(mouse - s->position));
			}
			else
			{
				s->ApplyLinearForce((mouse - s->position));
			}
			//s->position = mouse;
		}
	}

	// RMB hold -> cuestick
	if (m_game->inputManager.isKeyDown(SDL_BUTTON_RIGHT))
	{
		if (!m_selectedShape.expired())
		{
			auto s = m_selectedShape.lock();
			//stop shape from moving
			s->velocity = { 0.0f, 0.0f };
			s->velocityAng = 0.0f;
			s->resetAcceleration();
		}
	}

	// LMB UP -> release object
	if (m_game->inputManager.isKeyReleased(SDL_BUTTON_LEFT))
	{
		m_selectedShape.reset();
	}

	// RMB UP -> apply acceleration
	if (m_game->inputManager.isKeyReleased(SDL_BUTTON_RIGHT))
	{
		if (!m_selectedShape.expired())
		{
			// lock the shape
			auto s = m_selectedShape.lock();
			glm::vec2 force = 5.0f * (s->position - m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords()) );
			s->ApplyLinearForce(force);
			glm::vec2 AngMomentum = 50.0f * (m_torquePoint - m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords()) );
			s->ApplyTorqueToPoint(m_torquePoint, AngMomentum );
			m_selectedShape.reset();
		}
	}
}

void GameplayScreen::handleWrapping()
{
	for (auto b : m_physicsSystem->getRigidbodyList())
	{
		if (b->position.x <= -m_window->getScreenWidth() / 2.0f - b->width)
			b->position.x = m_window->getScreenWidth() / 2.0f + b->width;
		else if (b->position.x >= m_window->getScreenWidth() / 2.0f + b->width)
			b->position.x = -m_window->getScreenWidth() / 2.0f - b->width;
		else if (b->position.y <= -m_window->getScreenHeight() / 2.0f - b->height)
			b->position.y = m_window->getScreenHeight() / 2.0f + b->height;
	}
}

bool GameplayScreen::onExitClicked(const CEGUI::EventArgs& e) {
	
    m_currentState = Gutengine::ScreenState::EXIT_APPLICATION;
    return true;
}