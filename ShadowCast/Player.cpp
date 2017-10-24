#include "Player.h"
#include <Gutengine\ResourceManager.h>
#include <SDL\SDL.h>
#include <iostream>


Player::Player()
{
}


Player::~Player()
{
}

void Player::init(b2World* world, const glm::vec2 position, const glm::vec2 dimensions, Gutengine::ColorRGBA8 color)
{
	Gutengine::GLTexture m_texture = Gutengine::ResourceManager::getTexture("Assets/survivor-idle_rifle_0.png");
	m_collisionBox.init(world, position, dimensions, m_texture, color, true, b2_dynamicBody);

	
}

void Player::draw(Gutengine::SpriteBatch& spriteBatch)
{
	m_collisionBox.draw(spriteBatch);
}

void Player::update(Gutengine::InputManager& inputManager, Gutengine::Camera2D camera)
{
	b2Body* body = m_collisionBox.getBody(); // reference to body

	if (inputManager.isKeyDown(SDLK_a)) {
		body->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
	} 
	else if (inputManager.isKeyDown(SDLK_d)) {
		body->ApplyForceToCenter(b2Vec2(100.0f, 0.0f), true);
	}
	if (inputManager.isKeyDown(SDLK_w)) {
		body->ApplyForceToCenter(b2Vec2(0.0f, 100.0f), true);
	}
	else if (inputManager.isKeyDown(SDLK_s)) {
		body->ApplyForceToCenter(b2Vec2(0.0f, -100.0f), true);
	}
	else { // apply damping - if no button is pressed, deaccelerate the player
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x * 0.95f, body->GetLinearVelocity().y * 0.95f));
	}
	
	// set maximum speed in the x direction
	if (body->GetLinearVelocity().x < -PLAYER_MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(-PLAYER_MAX_SPEED, body->GetLinearVelocity().y));
	}
	else if (body->GetLinearVelocity().x > PLAYER_MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(PLAYER_MAX_SPEED, body->GetLinearVelocity().y));
	}

	//set maximum speed in the y direction
	if (body->GetLinearVelocity().y < -PLAYER_MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2( body->GetLinearVelocity().x, -PLAYER_MAX_SPEED));
	}
	else if (body->GetLinearVelocity().y > PLAYER_MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, PLAYER_MAX_SPEED));
	}

	//get mouse coordinates and set turn the player sprite to face there
	
	glm::vec2 mouseCoord = camera.convertScreenToWorld(inputManager.getMouseCoords());

	glm::vec2 playerCoord = glm::vec2(body->GetPosition().x, body->GetPosition().y);


	m_direction = glm::normalize(mouseCoord - playerCoord);

	// get the angle to the desired location
	float desiredAngle = atan2f(-m_direction.x, m_direction.y);
	

	// set the angle of the collision box, where the sprite resides
	body->SetTransform(body->GetPosition(),desiredAngle + 1.5f);


}
