#include "GutPhysics2D.h"
#include <iostream>

namespace Gutengine
{

GutPhysics2D::GutPhysics2D()
{
}

GutPhysics2D::~GutPhysics2D()
{
}

void GutPhysics2D::updatePhysics()
{
	// container holding colliding pairs
	std::vector<std::pair<RigidBody2D*, RigidBody2D*>> collidingPairs;

	// update attributes
	for (auto& body : m_rigidBodies)
	{
		// update acceleration
		// add gravity 
		body->setLinearAcceleration(body->getLinearAcceleration() + getGravity());
		// update velocity
		body->setLinearVelocity(body->getLinearVelocity() + body->getLinearAcceleration());
		// update position
		body->setPosition(body->getPosition() + body->getLinearVelocity());

		// clamp items to ground for now
		if (body->getPosition().y < 0.0f)
			body->setPosition({ body->getPosition().x, 0.0f });
	}




	// Clamp velocity near zero

	// Check overlap and resolve static collisions

	for (auto &body : m_rigidBodies)
	{
		auto target = std::next(body); // next body in list

		if (checkAABBvsAABB(body, target))
		{
			collidingPairs.push_back(std::make_pair(body, target));

			// calculate overlap in x and y directions
			float xOverlap = fabsf(body->getPosition().x - target->getPosition().x);
			float yOverlap = fabsf(body->getPosition().y - target->getPosition().y);
			// distance needed to displace the bodies
			float distX = 0.5f * xOverlap;
			float distY = 0.5f * yOverlap;

			// if body is higher than target
			if (body->getPosition().y >= target->getPosition().y)
			{
				body->setPosition({ body->getPosition().x, body->getPosition().y + distY });
				target->setPosition({ body->getPosition().x, target->getPosition().y - distY });
			}
			else // reverse the displacement directions
			{
				body->setPosition({ body->getPosition().x, body->getPosition().y - distY });
				target->setPosition({ body->getPosition().x, target->getPosition().y + distY });
			}

			// if body is left of target
			if (body->getPosition().x >= target->getPosition().x)
			{
				body->setPosition({ body->getPosition().x + distX, body->getPosition().y });
				target->setPosition({ target->getPosition().x - distX, target->getPosition().y });
			}
			else // reverse the displacement directions
			{
				body->setPosition({ body->getPosition().x - distX, body->getPosition().y });
				target->setPosition({ target->getPosition().x + distX, target->getPosition().y });
			}
		}
	}

	// resolve dynamic collisions for colliding objects
	/*
	for (auto c : collidingPairs)
	{
		RigidBody2D* b1 = c.first;
		RigidBody2D* b2 = c.second;

		// resolution from wikipedia 

		// normal
		float n = ()
		glm::vec2 k = b1->getLinearVelocity() - b2->getLinearVelocity(); // k


	}
	*/


}

void GutPhysics2D::addRigidBody2d(RigidBody2D &obj)
{
	m_rigidBodies.push_back(new RigidBody2D(obj));
}

void GutPhysics2D::destroy()
{
	m_rigidBodies.clear();
	m_particles.clear();
}

bool GutPhysics2D::checkAABBvsAABB(const RigidBody2D *lhs, const RigidBody2D *rhs)
{
	// lhs values
	float r1X = lhs->getPosition().x;
	float r1Y = lhs->getPosition().y;
	float r1Width = lhs->getWidth();
	float r1Height = lhs->getHeight();
	// rhs values
	float r2X = rhs->getPosition().x;
	float r2Y = rhs->getPosition().y;
	float r2Width = rhs->getWidth();
	float r2Height = rhs->getHeight();

	
	if (r1X < r2X + r2Width &&
		r1X + r1Width > r2X &&
		r1Y < r2Y + r2Height &&
		r1Y + r1Height > r2Y) 
	{
		// collision detected
		return true;
	}

	else {
		// no collision
		return false;
	}
}

bool GutPhysics2D::checkSameSide(glm::vec2 point1, glm::vec2 point2, glm::vec2 a, glm::vec2 b)
{
	// homogenize the points
	glm::vec3 hpoint1 = { point1.x, point1.y, 0.0f };
	glm::vec3 hpoint2 = { point2.x, point2.y, 0.0f };
	glm::vec3 ha =		{ a.x, a.y, 0.0f };
	glm::vec3 hb =		{ b.x, b.y, 0.0f };

	// cross products
	glm::vec3 cp1 = glm::cross(hb - ha, hpoint1 - ha);
	glm::vec3 cp2 = glm::cross(hb - ha, hpoint2 - ha);

	return (glm::dot(cp1, cp2) >= 0.0f);
}

bool GutPhysics2D::checkPointInTringle(glm::vec2 point, glm::vec2 a, glm::vec2 b, glm::vec2 c)
{
	if (checkSameSide(point, a, b, c) &&
		checkSameSide(point, b, a, c) &&
		checkSameSide(point, c, a, b))
	{
		return true;
	}
	return false;
}

bool GutPhysics2D::checkPointInRigidBody(glm::vec2 point, RigidBody2D & rect)
{
	if (checkPointInTringle(point, rect.getBLCorner(), rect.getTRCorner(), rect.getTLCorner()) &&
		checkPointInTringle(point, rect.getBLCorner(), rect.getBRCorner(), rect.getTRCorner()))
	{
		return true;
	}
	return false;
}

} // namespace end
