#pragma once
#include "GameObject.h"
class Monster : 
	public GameObject
{
public:
	Monster(
		b2World* world,
		const glm::vec2 position,
		const glm::vec2 dimensions,
		Gutengine::GLTexture texture,
		Gutengine::ColorRGBA8 color,
		bool fixedRotation,
		b2BodyType bodyType,
		glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	~Monster();

private:
	Gutengine::GLTexture m_texture;
	glm::vec2 m_dimensions;
	glm::vec4 m_uvRect;
	Gutengine::ColorRGBA8 m_color;
	b2Body* m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
};

