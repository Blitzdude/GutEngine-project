#pragma once
#include "DebugRenderer.h"
#include <glm/glm.hpp>
//
#include <vector>
namespace Gutengine
{
	struct AABB
	{
		// x,y - bottom left
		glm::vec2 pos;
		glm::vec2 dim;
	};

	class RigidBody
	{
	public:
		static unsigned int objectCount;

		glm::vec2 position;
		glm::vec2 velocity;
		glm::vec2 acceleration;
		float	  mass;
		int id;

		float orientation;
		float velocityAng;
		float accelerationAng;
		// SHAPE TYPE
		bool isStatic;

	public:
		virtual void Update(float deltaTime) = 0;
		virtual void DebugDraw(DebugRenderer & renderer) = 0;
		virtual bool PointInShape(glm::vec2 point) { return false; };
		virtual void ApplyLinearImpulse(glm::vec2 force) {};
		virtual void ApplyTorque(glm::vec2 force) {};

		// Getters
		virtual AABB GetAABB() = 0;
	};


	class Rectangle : public RigidBody
	{
	public:
		Rectangle(glm::vec2 pos, float w, float h);

		void Update(float deltaTime) override;
		void DebugDraw(DebugRenderer & renderer) override;
		bool PointInShape(glm::vec2 point) override;
		void ApplyLinearImpulse(glm::vec2 force) override;
		void ApplyTorque(glm::vec2 force) override;

		float width;
		float height;
		// corners
		glm::vec2 const getTLCorner() const
		{
			return { position.x, position.y + height };
		};

		glm::vec2 const getTRCorner() const
		{
			return{ position.x + width, position.y + height };
		};

		glm::vec2 const getBRCorner() const
		{
			return{ position.x + width, position.y };
		};

		glm::vec2 const getBLCorner() const
		{
			return{ position.x, position.y };
		};

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
	glm::vec2 acceleration;
	float mass;
};
/*
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
	std::vector<Gutengine::RigidBody2D*>& getRigidbodyList() { return m_rigidBodies; };
private:

	glm::vec2 m_gravity;
	
	std::vector<Gutengine::Particle2D*>  m_particles;
	std::vector<Gutengine::RigidBody2D*> m_rigidBodies;
};

*/
} // namespace end
