#include "Box.h"



Box::Box()
{
}


Box::~Box()
{
}

void Box::init(b2World * world, const glm::vec2 position, const glm::vec2 dimensions, Gutengine::GLTexture texture, Gutengine::ColorRGBA8 color, bool fixedRotation)
{
	m_dimensions = dimensions;
	m_color = color;
	m_texture = texture;

	// make body.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;					// declare as dynamic body, so that it can be affected by force
	bodyDef.position.Set(position.x, position.y);	// set bodys position
	bodyDef.fixedRotation = fixedRotation;			// sets if the body rotates
	m_body = world->CreateBody(&bodyDef);			// use world to create the body 

	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f); // Box2D creates box by half the length and width

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	m_fixture = m_body->CreateFixture(&fixtureDef);
}

void Box::draw(Gutengine::SpriteBatch& spriteBatch)
{
	glm::vec4 destRect;	// create the destination rectangle
	destRect.x = m_body->GetPosition().x - m_dimensions.x / 2.0f;
	destRect.y = m_body->GetPosition().y - m_dimensions.y / 2.0f;
	destRect.z = m_dimensions.x;
	destRect.w = m_dimensions.y;
	spriteBatch.draw(destRect,					// Destination rectangle
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),		// Uv-rectangle x,y,w,h
		m_texture.id, 0.0f,						// texture to draw
		m_color,								// color
		m_body->GetAngle());					// rotation of the spritebatch
}
