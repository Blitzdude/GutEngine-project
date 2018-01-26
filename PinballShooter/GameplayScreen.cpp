#include "GameplayScreen.h"
#include <SDL\SDL.h>
#include <iostream>
#include <Gutengine\IMainGame.h>
#include <Gutengine\ResourceManager.h>

#include <random>
#include <math.h>


GameplayScreen::GameplayScreen(Gutengine::Window * window) : m_window(window)
{
}


GameplayScreen::~GameplayScreen()
{
}

int GameplayScreen::getNextScreenIndex() const
{
	return SCREEN_INDEX_NO_SCREEN;
}

int GameplayScreen::getPreviousScreenIndex() const
{
	return SCREEN_INDEX_NO_SCREEN;
}

void GameplayScreen::build()
{
	std::cout << "Build: \n";
}

void GameplayScreen::destroy()
{
	std::cout << "Destroy: \n";
}

void GameplayScreen::onEntry()
{
	std::cout << "OnEntry: \n";

	// Initialize spritebatch
	m_spriteBatch.init();

	// Shader initialization
	initShaders();

	// Initialize debug Renderer
	m_debugRenderer.init();
	std::cout << "Debug Renderer initialized" << std::endl;

	b2Vec2 gravity(0.0f, 0.0f); //< we don't want gravity
	m_world = std::make_unique<b2World>(gravity);

	// create player
	m_player.init(m_world.get(),
		glm::vec2(0, 0),
		glm::vec2(10, 10),
		Gutengine::ResourceManager::getTexture("Assets/survivor-idle_rifle_0.png"),
		Gutengine::ColorRGBA8(255, 255, 255, 255),
		false,
		b2_dynamicBody,
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));


	// initialize camera.
	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(2.0f); // 32 pixels / meter

}

void GameplayScreen::onExit()
{
	std::cout << "OnExit: \n";
	m_debugRenderer.dispose();
}

void GameplayScreen::update()
{
	// Game Logic here

	// Update the camera
	m_camera.update();
	checkInput();
}

void GameplayScreen::draw()
{
	// Rendercode here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); ///< Clear gl color and depth buffers
	glClearColor(0.30f, 0.40f, 0.0f, 1.0f); ///< set Clear color to Solid red

	m_textureProgram.use();

	// upload texture uniform
	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1d(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	//camera matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_spriteBatch.begin();

	// Draw code here
	m_player.draw(m_spriteBatch);

	m_spriteBatch.end();

	m_spriteBatch.renderBatch();
	m_textureProgram.unuse();

	// Debug rendering
	if (m_renderDebug) {
		glm::vec4 destRect;
		destRect.x = 0;
		destRect.y = 0;
		destRect.z = 10;
		destRect.w = 10;


		m_debugRenderer.drawBox(destRect, Gutengine::ColorRGBA8(255, 0, 255, 255), 0.0f);
		glm::vec2 circle;
		circle.x = m_player.getBody()->GetPosition().x;
		circle.y = m_player.getBody()->GetPosition().y;
		m_debugRenderer.drawCircle(circle, Gutengine::ColorRGBA8(255, 255, 255, 255), m_player.getDimensions().x);
		// drawing end


		m_debugRenderer.end();
		m_debugRenderer.render(projectionMatrix, 2.0f);
		// debug rendering - end.
	}
}

void GameplayScreen::checkInput()
{
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
	}
}

void GameplayScreen::initShaders()
{
	// Compile our color shader
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();
}


