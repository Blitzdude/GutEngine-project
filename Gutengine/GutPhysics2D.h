#pragma once

#include <glm/glm.hpp>
namespace Gutengine
{
struct RigidBody2D
{
	// default constructor
	RigidBody2D() {}
	RigidBody2D(glm::vec2 _p, glm::vec2 _v, float _w = 1.0f, float _h = 1.0f, float _m = 1.0f, float _or = 0.0f )
	: width(_w), height(_h), mass(_m), orientation(_or)
	{
		position = _p;
		velocity = _v;
	}

	// attributes
	float width;
	float height;
	float mass;
	glm::vec2 position;
	glm::vec2 velocity;
	// orientation in rads
	float orientation;

};

struct Particle2D
{
	/// Default constructor
	Particle2D() {}
	/// Parameter constructors
	Particle2D(float x, float y, float vX = 0.0f, float vY = 0.0f, float m = 1.0f) : mass(m) 
	{
		position.x = x;
		position.y = y;
		velocity.x = vX;
		velocity.y = vY;
	}

	Particle2D(glm::vec2 _p, glm::vec2 _v, float m = 1.0f) : mass(m) 
	{ 
		position = _p; 
		velocity = _v; 
	}
	
	/// attributes
	glm::vec2 position;
	glm::vec2 velocity;
	float mass;
};

class GutPhysics2D
{
public:
	GutPhysics2D();
	~GutPhysics2D();
};

}
