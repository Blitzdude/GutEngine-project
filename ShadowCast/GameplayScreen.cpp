#include "GameplayScreen.h"
#include <SDL\SDL.h>
#include <iostream>
#include <Gutengine\IMainGame.h>
#include <Gutengine\ResourceManager.h>
#include <random>



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
	const int NUM_BOXES = 50;

	std::cout << "OnEntry: \n";
	b2Vec2 gravity(0.0f, -9.81f); //< we don't want gravity
	m_world = std::make_unique<b2World>(gravity);

	// make the ground - do not want
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -25.0f);

	//allocates body in memory
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef);
	// make the ground fixture - bind the shape of the body.
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

		// initialize random generator 
	std::mt19937 randGenerator;
		// could seed the random generator with time, but we wont
		// set distributions
	std::uniform_real_distribution<float> xDistPos(-10.0f, 10.0f);
	std::uniform_real_distribution<float> yDistPos(-15.0f, 15.0f);
	std::uniform_real_distribution<float> sizeDistribution(0.5f, 2.5f);
	std::uniform_int_distribution<int> ColorDist(0, 255);


	// Load textures;
	m_texture = Gutengine::ResourceManager::getTexture("Assets/bricks_top.png");

	//make a bunch of new boxes and push them into the boxes vector container
	for (int i = 0; i < NUM_BOXES; i++) {
		// randomize color
		Gutengine::ColorRGBA8 randColor;
		randColor.r = ColorDist(randGenerator);
		randColor.g = ColorDist(randGenerator);
		randColor.b = ColorDist(randGenerator);
		randColor.a = 255;

		// create a new box with random pos(x,y), size(w,h) and color(r,g,b), set fixedRotation to false
		Box newBox;
		newBox.init(m_world.get(),
			glm::vec2(xDistPos(randGenerator), yDistPos(randGenerator)),
			glm::vec2(sizeDistribution(randGenerator), sizeDistribution(randGenerator)),
			m_texture,
			randColor,
			false);
		// push new created box to vector container
		m_boxes.push_back(newBox);
	}

	

	// Initialize spritebatch
	m_spriteBatch.init();

	// Shader initialization
	initShaders();
	
	

	// initialize camera.
	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(32.0f); // 32 pixels / meter

	// Initialize player
	m_player.init(m_world.get(), glm::vec2(0.0f, 30.0f), glm::vec2(1.0f, 2.0f), m_texture, Gutengine::ColorRGBA8(255, 255, 255, 255));
}

void GameplayScreen::onExit()
{
	std::cout << "OnExit: \n";
}

void GameplayScreen::update()
{
	static int counter = 0;
	std::cout << "Update: " << ++counter << "\n";

	// Update the camera
	m_camera.update();
	checkInput();

	// update The physics simulation in BOX2D world
	m_world->Step(1.0f / 60.0f, 6, 2);
}

void GameplayScreen::draw()
{

	std::cout << "Draw: \n";
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); ///< Clear gl color and depth buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); ///< set Clear color to Solid red

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

	// draw all the boxes
	for (auto b : m_boxes) {
		b.draw(m_spriteBatch);
	}

	// draw the player
	m_player.draw(m_spriteBatch);

	m_spriteBatch.end();

	m_spriteBatch.renderBatch();
	m_textureProgram.unuse();
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
