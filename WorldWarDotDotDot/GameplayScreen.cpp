#include "GameplayScreen.h"
#include <iostream>
#include <SDL/SDL.h>
#include <Gutengine/IMainGame.h>
#include <Gutengine/ResourceManager.h>
#include <random>

#include "ScreenIndices.h"

//#include "Light.h"

// GLOBALS
const int CELL_SIZE = 12;

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

    b2Vec2 gravity(0.0f, 0.0f);
    m_world = std::make_unique<b2World>(gravity);

	// Initialize grid
	m_grid = std::make_unique<Grid>(m_window->getScreenWidth(), m_window->getScreenHeight(), CELL_SIZE);

	// Initialize debug renderer
    m_debugRenderer.init();

    // Initialize spritebatch
    m_spriteBatch.init();

    // Shader init
    // Compile our texture
    m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    m_textureProgram.addAttribute("vertexPosition");
    m_textureProgram.addAttribute("vertexColor");
    m_textureProgram.addAttribute("vertexUV");
    m_textureProgram.linkShaders();

    // Init camera

    m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	// move the camera, so the world origin is in the bottom-left corner
	m_camera.setPosition(glm::vec2(m_window->getScreenWidth() / 2.0f, m_window->getScreenHeight() / 2.0f));

    initUI();
}

void
GameplayScreen::onExit() {
    m_debugRenderer.dispose();
    m_world.reset();
}

void
GameplayScreen::update() {
    m_camera.update();
    checkInput();

    // Update the physics simulation
    m_world->Step(1.0f / 60.0f, 6, 2);
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

    m_spriteBatch.begin();

    m_spriteBatch.end();
	// add grid rendering here
    m_spriteBatch.renderBatch();
    m_textureProgram.unuse();

    // Debug rendering
    if (m_renderDebug) {
        glm::vec4 destRect;
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
		
		m_debugRenderer.drawBox(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), Gutengine::ColorRGBA8(255, 0, 255, 255), 0.0f);
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
    CEGUI::PushButton* testButton = static_cast<CEGUI::PushButton*>(m_gui.createWidget("TaharezLook/Button", glm::vec4(0.01f, 0.01f, 0.1f, 0.05f), glm::vec4(0.0f), "TestButton"));
    testButton->setText("Exit Game!");

    // Set the event to be called when we click
    testButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameplayScreen::onExitClicked, this));

    //m_gui.setMouseCursor("TaharezLook/MouseArrow");
    //m_gui.showMouseCursor();
    SDL_ShowCursor(1);
}

void
GameplayScreen::checkInput() {
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

bool
GameplayScreen::onExitClicked(const CEGUI::EventArgs& e) {
    m_currentState = Gutengine::ScreenState::EXIT_APPLICATION;
    return true;
}