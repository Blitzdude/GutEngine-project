#include "Pinball.h"
#include <Gutengine\ResourceManager.h>
#include <SDL\SDL.h>
#include <iostream>

Pinball::Pinball()
{
}


Pinball::~Pinball()
{
}

void Pinball::init(b2World * world, const glm::vec2 position, const float radius, Gutengine::ColorRGBA8 color) 
{
	Gutengine::GLTexture m_texture = Gutengine::ResourceManager::getTexture("Assets/pinball.png");
	m_collisionCircle.init(world, position, radius, m_texture, color, true, b2_dynamicBody);
	m_collisionCircle.getFixture()->SetDensity(0.01f);
	m_collisionCircle.getFixture()->SetFriction(0.0f);
	m_collisionCircle.getFixture()->SetRestitution(1.0f);

	m_collisionCircle.getBody()->SetUserData(this);
	m_contacts = 0;
}

void Pinball::draw(Gutengine::SpriteBatch & spriteBatch)
{
	if (m_contacts != 0)
		m_collisionCircle.setColor(Gutengine::ColorRGBA8(255, 255, 255, 255));
	else
		m_collisionCircle.setColor(Gutengine::ColorRGBA8(255, 0, 0, 255));

	m_collisionCircle.draw(spriteBatch);
}

void Pinball::update(Gutengine::InputManager & inputManager, Gutengine::Camera2D camera)
{
	b2Body* body = m_collisionCircle.getBody(); // reference to body
												// update enemy movement and other stuff;

	// apply maximum speed ////
	// set maximum speed in the x direction
	if (body->GetLinearVelocity().x < -PINBALL_MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(-PINBALL_MAX_SPEED, body->GetLinearVelocity().y));
	}
	else if (body->GetLinearVelocity().x > PINBALL_MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(PINBALL_MAX_SPEED, body->GetLinearVelocity().y));
	}

	//set maximum speed in the y direction
	if (body->GetLinearVelocity().y < -PINBALL_MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, -PINBALL_MAX_SPEED));
	}
	else if (body->GetLinearVelocity().y > PINBALL_MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, PINBALL_MAX_SPEED));
	}

}

void Pinball::startContact()
{
	m_contacts++;
}

void Pinball::endContact()
{
	m_contacts--;
}

const glm::vec2 Pinball::getPosition() const
{
	return glm::vec2(m_collisionCircle.getBody()->GetPosition().x,
		m_collisionCircle.getBody()->GetPosition().y);
}
