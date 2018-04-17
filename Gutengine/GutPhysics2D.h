#pragma once

#include <glm/glm.hpp>
//
#include <vector>
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
		m_position.x = _p.x + (m_width / 2.0f);
		m_position.y = _p.y + (m_height / 2.0f);
		m_velocity = _v;

	}
	// destructor
	~RigidBody2D() {};

	// getters
	float const getWidth()				const { return m_width; };
	float const getHeight()				const { return m_height; };
	float const getMass()				const { return m_mass; };
	glm::vec2 const getPosition()		const { return m_position; };
	glm::vec2 const getLinearVelocity() const { return m_velocity; };
	float const getOrientation()		const { return m_orientation; };
	// corners
	glm::vec2 const getTLCorner() const 
	{
		return {m_position.x + (m_width / 2.0f), m_position.y - (m_height / 2.0f)};
	};

	glm::vec2 const getTRCorner() const 
	{ 
		return{ m_position.x - (m_width / 2.0f), m_position.y - (m_height / 2.0f) };
	};

	glm::vec2 const getBRCorner() const 
	{
		return{ m_position.x - (m_width / 2.0f), m_position.y + (m_height / 2.0f) };
	};

	glm::vec2 const getBLCorner() const 
	{
		return{ m_position.x + (m_width / 2.0f), m_position.y + (m_height / 2.0f) };
	};

	
	// setters
	void setWidth(const float other)				{ m_width = other; };
	void setHeight(const float other)				{ m_height = other; };
	void setMass(const float other)					{ m_mass = other; };
	void setPosition(const glm::vec2 &other)		{ m_position = other; };
	void setX(const float other)					{ m_position.x = other; };
	void setY(const float other)					{ m_position.y = other; };
	void setLinearVelocity(const glm::vec2 &other)	{ m_velocity = other; };
	void setOrientation	  (const float other)		{ m_orientation = other; };

private:
	// attributes
	float m_width;
	float m_height;
	float m_mass;
	//
	glm::vec2 m_position;
	// corners
	//
	glm::vec2 m_velocity;
	// orientation in rads
	float m_orientation;

};

class Particle2D
{
public:
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

	// destructor
	~Particle2D() {};
	
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

	void updatePhysics();

	void addRigidBody2d(RigidBody2D &obj);
	void addParticle();

	void destroy();
	// collision detection methods
	static bool checkAABBvsAABB(const RigidBody2D *lhs, const RigidBody2D *rhs);
	static bool checkRvsR(glm::vec2 &lhs, glm::vec2 &rhs);
	static bool checkAABBvsR(RigidBody2D &lhs, glm::vec2 &rhs);
	//
	static bool checkSameSide(glm::vec2 point1, glm::vec2 point2, glm::vec2 a, glm::vec2 b);
	static bool checkPointInTringle(glm::vec2 point, glm::vec2 a, glm::vec2 b, glm::vec2 c);
	static bool checkPointInRigidBody(glm::vec2 point, RigidBody2D &rect);


	// setters
	void setGravity(const glm::vec2 &value) { m_gravity = value; };
	// getter
	const glm::vec2& getGravity() const { return m_gravity; }; 
	const std::vector<Gutengine::RigidBody2D*>& getRigidbodyList() const { return m_rigidBodies; };
private:

	glm::vec2 m_gravity;
	std::vector<Gutengine::Particle2D*>  m_particles;
	std::vector<Gutengine::RigidBody2D*> m_rigidBodies;
};

}
