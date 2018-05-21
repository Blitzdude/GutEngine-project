#include "Object.h"
#include <Gutengine/ResourceManager.h>


Object::Object()
{
}


Object::~Object()
{
}

void Object::destroy(b2World &world)
{
	m_body->DestroyFixture(m_fixture);
	world.DestroyBody(m_body);
}


