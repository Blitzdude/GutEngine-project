#pragma once
#include <glm/glm.hpp>
#include <Box2D/Box2D.h>
#include <Gutengine/GLTexture.h>
#include <Gutengine/SpriteBatch.h>
#include "Grid.h"

class Object
{
public:
	Object();
	~Object();

	void init(glm::vec2 pos, float or, float r, b2World &world );
	void destroy(b2World &world);
	void draw(Gutengine::SpriteBatch &spriteBatch);
	void update(Grid &grid);

private:
	// box2D
	b2Body* m_body;
	b2Fixture* m_fixture;
	
	glm::vec2 m_position;
	float m_orientation;
	float m_radius;

	Gutengine::GLTexture m_texture;

	bool isDynamic = true;
	bool hasFixedRotation = false;
};

