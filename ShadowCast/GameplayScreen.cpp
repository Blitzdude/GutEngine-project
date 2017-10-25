#include "GameplayScreen.h"
#include <SDL\SDL.h>
#include <iostream>
#include <Gutengine\IMainGame.h>
#include <Gutengine\ResourceManager.h>

#include <random>
#include <math.h>


const float VISION_RADIUS = 20.0f;
const float PI = 3.14159265359f;

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
	const int NUM_BOXES = 3;

	std::cout << "OnEntry: \n";

	// Initialize spritebatch
	m_spriteBatch.init();

	// Shader initialization
	initShaders();
	
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
	std::uniform_real_distribution<float> xDistPos(-5.0f, 5.0f);
	std::uniform_real_distribution<float> yDistPos(-5.0f, 5.0f);
	std::uniform_real_distribution<float> sizeDistribution(2.5f, 4.5f);
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

	// make enemies
	Enemy enemy1;
	enemy1.init(m_world.get(), glm::vec2(-0.5f, 0.0f), 1.0f, Gutengine::ColorRGBA8(255, 255, 255, 255));
	m_enemies.push_back(enemy1);
	
	// initialize camera.
	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(32.0f); // 32 pixels / meter

	// Initialize player
	m_player.init(m_world.get(), glm::vec2(0.0f, 0.0f), glm::vec2(1.5f, 1.5f), Gutengine::ColorRGBA8(255, 255, 255, 255));

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

	// RAYCASTING 
	// clear the raycastResults container, so it won't grow infinitely
	m_callbackResults.clear();

	static const float OFFSET_ANGLE = 0.0001f;
	b2Vec2 v1, v2;

	// Players position vector v1
	v1.x = m_player.getBox().getBody()->GetPosition().x;
	v1.y = m_player.getBox().getBody()->GetPosition().y;

	// convert vector to glm

	for (auto& box_itr : m_boxes) {
	// ray cast to all corners within radius
			
		for(auto& corner_itr : box_itr.getCorners()) {
			v2.x = corner_itr.x;
			v2.y = corner_itr.y;
			b2Vec2 vLeft, vRight;
			

			if (b2Distance(v1, v2) < VISION_RADIUS) {

				// center
				m_world->RayCast(&m_rcCallback, v1, v2);
				m_callbackResults.push_back(m_rcCallback.m_point);

				// calculate left offset
				vLeft.x = v2.x * cosf(OFFSET_ANGLE) - v2.y * sinf(OFFSET_ANGLE);
				vLeft.y = v2.x * sinf(OFFSET_ANGLE) + v2.y * cosf(OFFSET_ANGLE);
				// left 
				m_world->RayCast(&m_rcCallback, v1, vLeft);
				m_callbackResults.push_back(m_rcCallback.m_point);

				// calculate right offset
				vRight.x = v2.x * cosf(-OFFSET_ANGLE) - v2.y * sinf(-OFFSET_ANGLE);
				vRight.y = v2.x * sinf(-OFFSET_ANGLE) + v2.y * cosf(-OFFSET_ANGLE);
				// right
				m_world->RayCast(&m_rcCallback, v1, vRight);
				m_callbackResults.push_back(m_rcCallback.m_point);
			}
		}
	}
	// Ray cast towards the corners by adding vision radius to the player position
	
	// topright ++
	m_world->RayCast(&m_rcCallback, v1, b2Vec2((v1.x + VISION_RADIUS), (v1.y + VISION_RADIUS)));
	m_callbackResults.push_back(m_rcCallback.m_point);
	// topleft -+
	m_world->RayCast(&m_rcCallback, v1, b2Vec2((v1.x - VISION_RADIUS), (v1.y + VISION_RADIUS)));
	m_callbackResults.push_back(m_rcCallback.m_point);
	// bottomright +-
	m_world->RayCast(&m_rcCallback, v1, b2Vec2((v1.x + VISION_RADIUS), (v1.y - VISION_RADIUS)));
	m_callbackResults.push_back(m_rcCallback.m_point);
	// bottomleft --
	m_world->RayCast(&m_rcCallback, v1, b2Vec2((v1.x - VISION_RADIUS), (v1.y - VISION_RADIUS)));
	m_callbackResults.push_back(m_rcCallback.m_point);
	
	
	
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

	auto p = m_player.getBox();

	// draw all the boxes
	for (auto b : m_boxes) {
		b.draw(m_spriteBatch);
	}

	// draw the player
	m_player.draw(m_spriteBatch);

	// draw the enemies
	for (auto itr : m_enemies) {
		if (b2Distance(p.getBody()->GetPosition(), itr.getCircle().getBody()->GetPosition()) < VISION_RADIUS) {
			itr.draw(m_spriteBatch);

		}
	}

	m_spriteBatch.end();

	m_spriteBatch.renderBatch();
	m_textureProgram.unuse();

	// Debug rendering
	if (m_renderDebug) {
		glm::vec4 destRect;
		// Render obstacle box collision boxes
		for (auto& b : m_boxes) {
			destRect.x = b.getBody()->GetPosition().x - b.getDimensions().x / 2.0f;
			destRect.y = b.getBody()->GetPosition().y - b.getDimensions().y / 2.0f;
			destRect.z = b.getDimensions().x;
			destRect.w = b.getDimensions().y;
			m_debugRenderer.drawBox(destRect, Gutengine::ColorRGBA8(255, 255, 255, 255), b.getBody()->GetAngle());
		}
		//render player collision box
		
		
		destRect.x = p.getBody()->GetPosition().x - p.getDimensions().x / 2.0f;
		destRect.y = p.getBody()->GetPosition().y - p.getDimensions().y / 2.0f;
		destRect.z = p.getDimensions().x;
		destRect.w = p.getDimensions().y;
		m_debugRenderer.drawBox(destRect, Gutengine::ColorRGBA8(255, 255, 255, 255), p.getBody()->GetAngle());
		

		// draw player vision radius
		
		glm::vec2 circleCenter;
		circleCenter.x = p.getBody()->GetPosition().x;
		circleCenter.y = p.getBody()->GetPosition().y;
		m_debugRenderer.drawCircle(circleCenter, Gutengine::ColorRGBA8(255, 255, 0, 255 ), VISION_RADIUS);
		


		// draw lines raycast results
		
		for (auto& it : m_callbackResults) {
			glm::vec2 v1, v2;

			// Players position vector v1
			v1.x = p.getBody()->GetPosition().x;
			v1.y = p.getBody()->GetPosition().y;

			v2.x = it.x;
			v2.y = it.y;

			m_debugRenderer.drawLine(v1, v2, Gutengine::ColorRGBA8(255, 0, 0, 255));	
		}
		
		

		// draw the polygon from raycast results
		// make glm format
		std::vector<glm::vec2> toDraw;
		for (auto& itr : m_callbackResults) {
			toDraw.push_back(glm::vec2(itr.x, itr.y));
		}

		m_debugRenderer.drawPolygon(toDraw, Gutengine::ColorRGBA8(0, 0, 255, 255), toDraw.size());
		
		
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

void GameplayScreen::makeLevelEdges()
{
	makeEdge(0.0f, 25.0f, 50.0f, 10.0f); // top edge
	makeEdge(0.0f, -25.0f, 50.0f, 10.0f); // bottom edge
	makeEdge(-35.0f, 0.0f,  10.0f, 50.0f); // left edge
	makeEdge(35.0f, 0.0f,  10.0f, 50.0f); // right edge
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