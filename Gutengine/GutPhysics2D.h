#pragma once
#include "DebugRenderer.h"
#include <glm/glm.hpp>
//
#include <vector>
#include <memory>

// TODO: Change all function names to camelBack
namespace Gutengine
{
	// Forward declare classes
	class Rectangle;

	struct AABB
	{
		// x,y - center
		// w,h - full
		glm::vec2 pos;
		float w;
		float h;

		bool isPointIn(glm::vec2 point)
		{
			return (point.x >= pos.x - w / 2.0f && point.x <= pos.x + w / 2.0f &&
				point.y >= pos.y - h / 2.0f && point.y <= pos.y + h / 2.0f);
		}
		bool overlaps(const AABB & other)
		{
			return (pos.x - w / 2.0f <= other.pos.x + other.w / 2.0f && pos.x + w / 2.0f >= other.pos.x - other.w / 2.0f &&
					pos.y - h / 2.0f <= other.pos.y + other.h / 2.0f && pos.y + h / 2.0f >= other.pos.y - other.h / 2.0f);
		}
	};
	
	struct SatManifold
	{
		SatManifold()
		{
			axis = { 0.0f, 0.0f };
			contactPoint = { 0.0f, 0.0f };
			length = 0.0f;
			left = nullptr;
			right = nullptr;
		};

		glm::vec2 axis;
		glm::vec2 contactPoint;
		float length;
		std::shared_ptr<Rectangle> left;
		std::shared_ptr<Rectangle> right;
	};

	class RigidBody
	{
	public:
		static unsigned int objectCount;

		float e = 0.0f; // Coefficient of restitution

		glm::vec2 position;
		glm::vec2 velocity;
		glm::vec2 acceleration;
		float mass = 1.0f;
		float angularMass = 1.0f;
		int	  id = -1; // -1 = not set

		float orientation = 0.0f;
		float velocityAng = 0.0f;
		float accelerationAng = 0.0f;
		// TODO: SHAPE TYPE enumerator variable
		bool isStatic = false;

	public:
		virtual void Update(float deltaTime) = 0; 
		virtual void DebugDraw(DebugRenderer & renderer) = 0;
		virtual bool PointInShape(glm::vec2 point) { return false; };
		virtual void ApplyLinearForce(glm::vec2 force) {};
		virtual void ApplyTorque(float force) {};
		virtual void ApplyTorqueToPoint(glm::vec2 point, glm::vec2 force) {};
		virtual void resetAcceleration() = 0;

		// Getters
		virtual AABB GetAABB() = 0;
		virtual std::vector<glm::vec2> getUniqueNormals() const = 0;
	};


	class Rectangle : public RigidBody
	{
	public:
		Rectangle(glm::vec2 pos, float w, float h, float or = 0.0f, float m = 1.0f);
		Rectangle();

		void Update(float deltaTime) override;
		void DebugDraw(DebugRenderer & renderer) override;
		bool PointInShape(glm::vec2 point) override;
		void ApplyLinearForce(glm::vec2 force) override;
		void ApplyTorque(float force) override;
		void ApplyTorqueToPoint(glm::vec2 point, glm::vec2 force) override; // TODO: ApplyForceToPoint ?
		void resetAcceleration() override;
		AABB GetAABB() override;
		
		float width;
		float height;
		// corners
		/*
			x,y+h ---- x+w,y+h
			|tl		   tr|
			|            |
			|bl		   br|
			x,y ------ x+w,y
		*/
		glm::vec2 const getTLCorner() const;
		glm::vec2 const getTRCorner() const;
		glm::vec2 const getBRCorner() const;
		glm::vec2 const getBLCorner() const;
		std::vector<glm::vec2> getCorners() const;
		std::vector<glm::vec2> getUniqueNormals() const override;

		glm::vec2 const getLinearVelocityOfPoint(const glm::vec2 point) const;
		
		
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

class GutPhysics2D
{
public:
	GutPhysics2D();
	~GutPhysics2D();

	void updatePhysics(float deltaTime);

	void addRigidBody2D(glm::vec2 pos, float w, float h, float o = 0.0f);
	void addRigidBody2D(const Rectangle& obj);

	//void addParticle();

	//void destroy();

	// projection
	glm::vec2 projectShapeToAxis(const Rectangle& shape, const glm::vec2& axis) const;
	bool checkSatCollision( Rectangle & a, Rectangle & b, SatManifold & mtv);

	// setters
	void setGravity(const glm::vec2 &value) { m_gravity = value; };
	// getter
	const glm::vec2& getGravity() const { return m_gravity; }; 
	std::vector<std::shared_ptr<Gutengine::Rectangle>>& getRigidbodyList() { return m_rigidBodies; };
private:

	glm::vec2 m_gravity;
	
	//std::vector<std::shared_ptr<Gutengine::Particle2D>>  m_particles;
	std::vector<std::shared_ptr<Gutengine::Rectangle>> m_rigidBodies;
};

} // namespace end
