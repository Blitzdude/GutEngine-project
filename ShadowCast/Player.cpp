#include "Player.h"



Player::Player()
{
}


Player::~Player()
{
}

void Player::init(b2World* world, const glm::vec2 position, const glm::vec2 dimensions, Gutengine::GLTexture texture, Gutengine::ColorRGBA8 color)
{
	m_collisionBox.init(world, position, dimensions, texture, color, true);
}

void Player::draw(Gutengine::SpriteBatch& spriteBatch)
{
	m_collisionBox.draw(spriteBatch);
}

void Player::update()
{
}
