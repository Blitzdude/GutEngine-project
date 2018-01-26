#include "Monster.h"



Monster::Monster(
	b2World* world,
	const glm::vec2 position,
	const glm::vec2 dimensions,
	Gutengine::GLTexture texture,
	Gutengine::ColorRGBA8 color,
	bool fixedRotation,
	b2BodyType bodyType,
	glm::vec4 uvRect /*= glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)*/)
{
}


Monster::~Monster()
{
}
