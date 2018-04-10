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

bool GutPhysics2D::checkAABB(RigidBody2D & lhs, RigidBody2D & rhs)
{
	// lhs values
	float r1X = lhs.getPosition().x;
	float r1Y = lhs.getPosition().y;
	float r1Width = lhs.getWidth();
	float r1Height = lhs.getHeight();
	// rhs values
	float r2X = rhs.getPosition().x;
	float r2Y = rhs.getPosition().y;
	float r2Width = rhs.getWidth();
	float r2Height = rhs.getHeight();

	
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
