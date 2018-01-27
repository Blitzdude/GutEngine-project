#include "Circle.h"


Circle::Circle()
{
}

Circle::~Circle()
{
}

void Circle::init(b2World * world,
	const glm::vec2 position,
	float radius,
	Gutengine::GLTexture texture,
	Gutengine::ColorRGBA8 color,
	bool fixedRotation,
	b2BodyType bodyType,
	glm::vec4 uvRect /*= glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)*/)
{
	m_dimensions.x = radius;
	m_dimensions.y = radius;
	m_color = color;
	m_texture = texture;
	m_uvRect = uvRect;

	// make body.
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position.Set(position.x, position.y);	// set bodys position
	bodyDef.fixedRotation = fixedRotation;			// sets if the body rotates
	m_body = world->CreateBody(&bodyDef);			// use world to create the body 

	b2CircleShape circleShape;
	circleShape.m_radius = radius / 2.0f;
	circleShape.m_p.Set(0.0f,0.0f); // Box2D creates box by half the length and width


	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	m_fixture = m_body->CreateFixture(&fixtureDef);
}

void Circle::draw(Gutengine::SpriteBatch& spriteBatch)
{
	glm::vec4 destRect;	// create the destination rectangle
	destRect.x = m_body->GetPosition().x - m_dimensions.x / 2.0f;
	destRect.y = m_body->GetPosition().y - m_dimensions.y / 2.0f;
	destRect.z = m_dimensions.x;
	destRect.w = m_dimensions.y;
	spriteBatch.draw(destRect,					// Destination rectangle
		m_uvRect,								// Uv-rectangle x,y,w,h
		m_texture.id, 0.0f,						// texture to draw
		m_color,								// color
		m_body->GetAngle());					// rotation of the spritebatch
}

