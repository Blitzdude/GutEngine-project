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

void Enemy::update(Gutengine::InputManager& inputManager, Gutengine::Camera2D camera)
{
	// empty
}
