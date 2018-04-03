#include "GameplayScreen.h"
#include <iostream>
#include <SDL/SDL.h>
#include <Gutengine/IMainGame.h>
#include <Gutengine/ResourceManager.h>
#include <random>

#include "ScreenIndices.h"


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

	// init particles
	glm::vec2 temp = (glm::vec2(100.0f, 100.0f));
	m_particles.push_back(Gutengine::Particle2D(temp, glm::vec2(-1.0f, 20.0f)));
	// Init rigidBodies
	m_rigidBodies.push_back(Gutengine::RigidBody2D(glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 0.0f)));

    initUI();
}

void GameplayScreen::onExit() {
    m_debugRenderer.dispose();
}

void GameplayScreen::update() {
	updateParticles();
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
       
		for (auto itr : m_particles) {
			m_debugRenderer.drawCircle(itr.position, Gutengine::ColorRGBA8(255, 0, 255, 255), 10.0f);
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
		static const float GUTPHYS_MAX_VELOCITY = 20.0f;
		glm::vec2 gravity = glm::vec2(0.0f, -0.3f);
		itr.velocity += gravity;

		if (itr.velocity.y > GUTPHYS_MAX_VELOCITY) {
			itr.velocity.y = GUTPHYS_MAX_VELOCITY;
		}
		else if (itr.velocity.y < (-GUTPHYS_MAX_VELOCITY)) {
			itr.velocity.y = -GUTPHYS_MAX_VELOCITY;
		}
		std::cout << "xV: " << itr.velocity.x << " yV: " << itr.velocity.y << std::endl;

		itr.position = itr.position + itr.velocity;

		if (itr.position.y < 0.0f ) {
			itr.position = m_camera.convertScreenToWorld(glm::vec2(1.0f, 200.0f));
		}
	}
}

bool GameplayScreen::onExitClicked(const CEGUI::EventArgs& e) {
    m_currentState = Gutengine::ScreenState::EXIT_APPLICATION;
    return true;
}