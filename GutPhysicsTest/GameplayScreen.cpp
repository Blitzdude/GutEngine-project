#include "GameplayScreen.h"
#include <iostream>
#include <SDL/SDL.h>
#include <Gutengine/IMainGame.h>
#include <Gutengine/ResourceManager.h>
#include <random>

#include "ScreenIndices.h"

#define PARTICLES_ON 0
#define RIGID_BOXES_ON 1

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
    m_camera.setScale(2.0f);

#if PARTICLES_ON

	// init particles
	glm::vec2 temp = (glm::vec2(100.0f, 100.0f));
	m_particles.push_back(Gutengine::Particle2D(temp, glm::vec2(-1.0f, 20.0f)));
#endif // PARTICLES_ON

	// Init rigidBodies
#if RIGID_BOXES_ON
	m_rigidBodies.push_back(Gutengine::RigidBody2D
	(glm::vec2(0.0f, 100.0f), // position
	glm::vec2(0.0f, 0.0f),  // velocity 
		100.0f,				// width	
		100.0f ));			// height
#endif // RIGID_BOXES_ON
    initUI();
}

void GameplayScreen::onExit() {
    m_debugRenderer.dispose();
}

void GameplayScreen::update() {

	if (PARTICLES_ON)
		updateParticles();
	if (RIGID_BOXES_ON)
		updateRigidbodies();

    m_camera.update();
    checkInput();

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
	for (auto itr : m_rigidBodies) {
		m_spriteBatch.draw(destRect, uvRect, Gutengine::ResourceManager::getTexture("assets/blank.png").id, 1.0f, 
			Gutengine::ColorRGBA8(255, 0, 126, 255));
	}
    m_spriteBatch.end();

    m_spriteBatch.renderBatch();
    m_textureProgram.unuse();

    // Debug rendering
    if (m_renderDebug) {
       
		#if PARTICLES_ON
		for (auto itr : m_particles) {
			m_debugRenderer.drawCircle(itr.position, Gutengine::ColorRGBA8(255, 0, 255, 255), 10.0f);
		}
		#endif // PARTICLES_ON

		#ifdef RIGID_BOXES_ON
		for (auto itr : m_rigidBodies) {
			glm::vec4 destRect;
			destRect.x = itr.getPosition().x;
			destRect.y = itr.getPosition().y;
			destRect.z = itr.getWidth();
			destRect.w = itr.getHeight();
			m_debugRenderer.drawBox(destRect, Gutengine::ColorRGBA8(255, 255, 255, 255), 0.0f);
		}
		#endif // RIGID_BOXES_ON
        
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
    CEGUI::PushButton* testButton = static_cast<CEGUI::PushButton*>(m_gui.createWidget("TaharezLook/Button", glm::vec4(0.01f, 0.01f, 0.1f, 0.05f), glm::vec4(0.0f), "TestButton"));
    testButton->setText("Exit Game!");

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
}


void GameplayScreen::updateParticles()
{
	for (auto&& itr : m_particles) {
		static const float GUTPHYS_MAX_VELOCITY = 5.0f;
		glm::vec2 gravity = { 0.0f, -0.3f }; // set gravity
		itr.velocity += gravity; // add gravity to particle

		// clamp velocity to max velocity
		if (itr.velocity.y > GUTPHYS_MAX_VELOCITY) {
			itr.velocity.y = GUTPHYS_MAX_VELOCITY;
		}
		else if (itr.velocity.y < (-GUTPHYS_MAX_VELOCITY)) {
			itr.velocity.y = -GUTPHYS_MAX_VELOCITY;
		}

		std::cout << "xV: " << itr.velocity.x << " yV: " << itr.velocity.y << std::endl;

		// move particle position
		itr.position = itr.position + itr.velocity;

		if (itr.position.y < 0.0f ) {
			itr.position = m_camera.convertScreenToWorld(glm::vec2(1.0f, 200.0f));
		}
	}
}

void GameplayScreen::updateRigidbodies()
{
	for (auto &&itr : m_rigidBodies) {
		glm::vec2 gravity = { 0.0f, -0.05f };
		//std::cout << "xV: " << itr.getLinearVelocity().x << " yV: " << itr.getLinearVelocity().y << std::endl;

		
		itr.setLinearVelocity((itr.getLinearVelocity() + gravity)) ;

		// move particles per position
		
		itr.setPosition(itr.getPosition() + itr.getLinearVelocity());


		if (itr.getPosition().y < -200.0f) {
			itr.setY(-200.0f);
			itr.setLinearVelocity(glm::vec2(0.0f, 0.0f));
		}
		
		std::cout << "x: " << itr.getPosition().x << " y: " << itr.getPosition().y << std::endl;

	}
}

bool GameplayScreen::onExitClicked(const CEGUI::EventArgs& e) {
    m_currentState = Gutengine::ScreenState::EXIT_APPLICATION;
    return true;
}