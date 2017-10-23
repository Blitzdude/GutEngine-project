#include "GameplayScreen.h"
#include <SDL\SDL.h>
#include <iostream>
#include <Gutengine\IMainGame.h>
#include <Gutengine\ResourceManager.h>

#include <random>


const float VISION_RADIUS = 15.0f;

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
	const int NUM_BOXES = 20;

	std::cout << "OnEntry: \n";

	// Initialize debug Renderer
	m_debugRenderer.init();

	b2Vec2 gravity(0.0f, 0.0f); //< we don't want gravity
	m_world = std::make_unique<b2World>(gravity);

	// make the level edges
	makeLevelEdges();

		// initialize random generator 
	std::mt19937 randGenerator;
		// could seed the random generator with time, but we wont
		// set distributions
	std::uniform_real_distribution<float> xDistPos(-10.0f, 10.0f);
	std::uniform_real_distribution<float> yDistPos(-15.0f, 15.0f);
	std::uniform_real_distribution<float> sizeDistribution(0.5f, 2.5f);
	std::uniform_int_distribution<int> ColorDist(0, 255);


	

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
			Gutengine::ResourceManager::getTexture("Assets/bricks_top.png"),
			randColor,
			true,
			b2_staticBody);
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
	m_player.init(m_world.get(), glm::vec2(0.0f, 0.0f), glm::vec2(2.0f, 2.0f), Gutengine::ColorRGBA8(255, 255, 255, 255));

}

void GameplayScreen::onExit()
{
	std::cout << "OnExit: \n";
	m_debugRenderer.dispose();
}

void GameplayScreen::update()
{
	

	// Update the player
	m_player.update(m_game->inputManager, m_camera);

	// Update the camera
	m_camera.update();
	checkInput();

	// update The physics simulation in BOX2D world
	m_world->Step(1.0f / 60.0f, 6, 2);


}

void GameplayScreen::draw()
{

	
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

	// Debug rendering
	if (m_renderDebug) {
		glm::vec4 destRect;
		for (auto& b : m_boxes) {
			destRect.x = b.getBody()->GetPosition().x - b.getDimensions().x / 2.0f;
			destRect.y = b.getBody()->GetPosition().y - b.getDimensions().y / 2.0f;
			destRect.z = b.getDimensions().x;
			destRect.w = b.getDimensions().y;
			m_debugRenderer.drawBox(destRect, Gutengine::ColorRGBA8(255, 255, 255, 255), b.getBody()->GetAngle());
		}
		//render player collision box
		auto p = m_player.getBox();
		destRect.x = p.getBody()->GetPosition().x - p.getDimensions().x / 2.0f;
		destRect.y = p.getBody()->GetPosition().y - p.getDimensions().y / 2.0f;
		destRect.z = p.getDimensions().x;
		destRect.w = p.getDimensions().y;
		m_debugRenderer.drawBox(destRect, Gutengine::ColorRGBA8(255, 255, 255, 255), p.getBody()->GetAngle());

		// draw player vision circle
		glm::vec2 circleCenter;
		circleCenter.x = p.getBody()->GetPosition().x;
		circleCenter.y = p.getBody()->GetPosition().y;
		m_debugRenderer.drawCircle(circleCenter, Gutengine::ColorRGBA8(255, 0, 0, 255 ), VISION_RADIUS);


		// render lines from player center to box corners
		for (auto& b : m_boxes) {
			glm::vec2 v1, v2;

			// Players position vector v1
			v1.x = p.getBody()->GetPosition().x;
			v1.y = p.getBody()->GetPosition().y;

			for (auto& c : b.getCorners()) { // getCorners returns a vector container with the corners
				// get vector to corner
				v2.x = c.x;
				v2.y = c.y;
				// see if the corner is within vision radius
				if (glm::length(v1 - v2) < VISION_RADIUS) {
					m_debugRenderer.drawLine(v1, v2, Gutengine::ColorRGBA8(255, 255, 255, 255));
				}
			}	
		}

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

void GameplayScreen::makeLevelEdges()
{
	makeEdge(0.0f, 25.0f, 50.0f, 10.0f); // top edge
	makeEdge(0.0f, -25.0f, 50.0f, 10.0f); // bottom edge
	makeEdge(-37.0f, 0.0f,  10.0f, 50.0f); // left edge
	makeEdge(37.0f, 0.0f,  10.0f, 50.0f); // right edge
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