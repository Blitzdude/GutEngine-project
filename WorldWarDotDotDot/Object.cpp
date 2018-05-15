#include "Object.h"
#include <Gutengine/ResourceManager.h>


Object::Object()
{
}


Object::~Object()
{
}

void Object::init(glm::vec2 pos, float or, float r, b2World &world)
{
	m_position = pos;
	m_orientation = or ;
	m_radius = r;

	// define the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;	// body is affected by forces
	bodyDef.position.Set(m_position.x, m_position.y);	// position
	bodyDef.fixedRotation = hasFixedRotation;	// is rotatable
	bodyDef.angle = m_orientation;	// starting rotation

	m_body = world.CreateBody(&bodyDef);

	// define the shape
	b2CircleShape circleShape;
	circleShape.m_radius = m_radius;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 0.20f;	// density
	fixtureDef.friction = 0.1f; // friction

	m_texture = Gutengine::ResourceManager::getTexture("Assets/human.png");
}

void Object::destroy(b2World &world)
{
	m_body->DestroyFixture(m_fixture);
	world.DestroyBody(m_body);
}

void Object::draw(Gutengine::SpriteBatch & spriteBatch)
{
	glm::vec4 destRect;
	destRect.x = m_body->GetPosition().x - m_radius / 2.0f;
	destRect.y = m_body->GetPosition().y - m_radius / 2.0f;
	destRect.z = m_radius;
	destRect.w = m_radius;
	spriteBatch.draw(destRect,
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), // x,y,w,h
		m_texture.id,
		0.0f,
		Gutengine::ColorRGBA8(255, 255, 255, 255),
		m_body->GetAngle());
}

void Object::update(Grid &grid)
{
	glm::vec2 pos = { m_body->GetPosition().x, m_body->GetPosition().y };
	Cell* cell = grid.getCell(pos);
	m_body->SetLinearVelocity(b2Vec2(cell->force.x * m_speed, cell->force.y * m_speed));
}
