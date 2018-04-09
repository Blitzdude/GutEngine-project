#pragma once

#include <glm/glm.hpp>
namespace Gutengine
{
class RigidBody2D
{
public:
	// default constructor
	RigidBody2D() {}
	RigidBody2D(glm::vec2 _p, glm::vec2 _v, float _w = 1.0f, float _h = 1.0f, float _m = 1.0f, float _or = 0.0f )
	: m_width(_w), m_height(_h), m_mass(_m), m_orientation(_or)
	{
		m_position = _p;
		m_velocity = _v;
	}
	// getters
	float const getWidth()  const { return m_width; };
	float const getHeight() const { return m_height; };
	float const getMass()   const { return m_mass; };
	glm::vec2 const getPosition()		const { return m_position; };
	glm::vec2 const getLinearVelocity() const { return m_position; };
	float const getOrientation() const { return m_orientation; };
	
	// setters
	void setWidth(const float other) { m_width = other; };
	void setHeight(const float other)				{ m_height = other; };
	void setMass(const float other)					{ m_mass = other; };
	void setPosition(const glm::vec2 &other)		{ m_position = other; };
	void setLinearVelocity(const glm::vec2 &other)	{ m_position = other; };
	void setOrientation	  (const float other)		{ m_orientation = other; };

private:
	// attributes
	float m_width;
	float m_height;
	float m_mass;
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	// orientation in rads
	float m_orientation;

};

class Particle2D
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
