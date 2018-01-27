#include "Enemy.h"
#include <Gutengine\ResourceManager.h>
#include <SDL\SDL.h>
#include <iostream>


Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}

void Enemy::init(b2World* world, const glm::vec2 position, const float radius, Gutengine::ColorRGBA8 color)
{
	Gutengine::GLTexture m_texture = Gutengine::ResourceManager::getTexture("Assets/zombie.png");
	m_collisionCircle.init(world, position, radius, m_texture, color, true, b2_dynamicBody);


}

void Enemy::draw(Gutengine::SpriteBatch& spriteBatch)
{
	m_collisionCircle.draw(spriteBatch);
}

void Enemy::update(Gutengine::InputManager& inputManager, Gutengine::Camera2D camera, glm::vec2 playerPosition)
{
	// update enemy movement and other stuff;

	glm::vec2 enemyPosition(m_collisionCircle.getBody()->GetPosition().x, m_collisionCircle.getBody()->GetPosition().y);

	m_direction = glm::normalize(playerPosition - enemyPosition);
	// get the angle to the desired location
	float desiredAngle = atan2f(-m_direction.x, m_direction.y);

	// set the angle of the collision box, where the sprite resides
	m_collisionCircle.getBody()->SetTransform(m_collisionCircle.getBody()->GetPosition(), desiredAngle + 1.5f);

	// move enemy towards player
	m_collisionCircle.getBody()->ApplyForceToCenter(b2Vec2(m_direction.x * ENEMY_MAX_SPEED, m_direction.y * ENEMY_SPEED) , true);

	// apply maximum speed
	// set maximum speed in the x direction
	if (m_collisionCircle.getBody()->GetLinearVelocity().x < -ENEMY_MAX_SPEED) {
		m_collisionCircle.getBody()->SetLinearVelocity(b2Vec2(-ENEMY_MAX_SPEED, m_collisionCircle.getBody()->GetLinearVelocity().y));
	}
	else if (m_collisionCircle.getBody()->GetLinearVelocity().x > ENEMY_MAX_SPEED) {
		m_collisionCircle.getBody()->SetLinearVelocity(b2Vec2(ENEMY_MAX_SPEED, m_collisionCircle.getBody()->GetLinearVelocity().y));
	}

	//set maximum speed in the y direction
	if (m_collisionCircle.getBody()->GetLinearVelocity().y < -ENEMY_MAX_SPEED) {
		m_collisionCircle.getBody()->SetLinearVelocity(b2Vec2(m_collisionCircle.getBody()->GetLinearVelocity().x, -ENEMY_MAX_SPEED));
	}
	else if (m_collisionCircle.getBody()->GetLinearVelocity().y > ENEMY_MAX_SPEED) {
		m_collisionCircle.getBody()->SetLinearVelocity(b2Vec2(m_collisionCircle.getBody()->GetLinearVelocity().x, ENEMY_MAX_SPEED));
	}

}
const glm::vec2 Enemy::getPosition() const
{
	return glm::vec2(m_collisionCircle.getBody()->GetPosition().x,
		m_collisionCircle.getBody()->GetPosition().y);
}