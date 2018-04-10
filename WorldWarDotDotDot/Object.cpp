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
	fixtureDef.density = 1.0f;	// density
	fixtureDef.friction = 0.1f; // friction

	m_texture = Gutengine::ResourceManager::getTexture("Assets/blank.png");
}

void Object::destroy(b2World &world)
{
	m_body->DestroyFixture(m_fixture);
	world.DestroyBody(m_body);
}

void Object::draw(Gutengine::SpriteBatch & spriteBatch)
{

}
