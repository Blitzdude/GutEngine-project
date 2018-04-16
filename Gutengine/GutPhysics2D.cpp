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
	//if colliding add forces
	// move bodies
	// check collisions 

	for (auto&& itr = m_rigidBodies.begin(); itr != m_rigidBodies.end() - 1; ++itr)
		for (auto&& itr_n = std::next(itr); itr_n != m_rigidBodies.end(); ++itr_n)
		{
			if (GutPhysics2D::checkAABBvsAABB( *itr, *itr_n) ) {
				(*itr)->setLinearVelocity((*itr_n)->getLinearVelocity() * (-1.0f));
				(*itr_n)->setLinearVelocity((*itr)->getLinearVelocity() * (-1.0f));

			}
		}


	for (auto itr : m_rigidBodies) {
		//std::cout << "xV: " << itr.getLinearVelocity().x << " yV: " << itr.getLinearVelocity().y << std::endl;


		(*itr).setLinearVelocity(((*itr).getLinearVelocity() + m_gravity));

		// move particles per position

		(*itr).setPosition((*itr).getPosition() + (*itr).getLinearVelocity());


		if ((*itr).getPosition().y < -200.0f) {
			(*itr).setY(-200.0f);
		}

		//std::cout << "x: " << (*itr).getPosition().x << " y: " << (*itr).getPosition().y << "\n";

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
		std::cout << "Collsion detected!" << std::endl;

		return true;
	}

	else {
		// no collision
		return false;
	}
}


} // namespace end
