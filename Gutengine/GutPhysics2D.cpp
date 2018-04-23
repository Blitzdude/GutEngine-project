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
	//check collision 

	for (auto&& itr = m_rigidBodies.begin(); itr != m_rigidBodies.end() - 1; ++itr) 
	{
		for (auto&& itr_n = std::next(itr); itr_n != m_rigidBodies.end(); ++itr_n)
		{
			if (GutPhysics2D::checkAABBvsAABB( *itr, *itr_n) ) 
			{
				/*
				(*itr)->setLinearVelocity((*itr)->getLinearVelocity() * (-1.0f));
				(*itr_n)->setLinearVelocity((*itr_n)->getLinearVelocity() * (-1.0f));
				*/
			}
		}
	}
	//if colliding resolve collisions

	// move bodies
	for (auto itr : m_rigidBodies) {
		
		(*itr).setLinearVelocity(((*itr).getLinearVelocity() + m_gravity));

		(*itr).setPosition((*itr).getPosition() + (*itr).getLinearVelocity());

		// ground
		if ((*itr).getPosition().y < -200.0f) {
			(*itr).setY(-200.0f);
		}

	}
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
