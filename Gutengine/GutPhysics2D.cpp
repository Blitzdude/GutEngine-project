#include "GutPhysics2D.h"
#include <iostream>
#include <algorithm>

namespace Gutengine
{
	Rectangle::Rectangle(glm::vec2 pos, float w, float h, float or/* = 0.0f */)
	{
		position.x = pos.x;
		position.y = pos.y;
		width = w;
		height = h;
		orientation = or;
	}
	Rectangle::Rectangle()
	{
		std::cout << "rect destroyed!" << std::endl;
	}
	
	GutPhysics2D::GutPhysics2D()
	{
	}

	GutPhysics2D::~GutPhysics2D()
	{
		m_rigidBodies.clear();
	}

	void GutPhysics2D::updatePhysics(float deltaTime)
	{
		// container holding colliding pairs
		//std::vector<std::pair<RigidBody*, RigidBody*>> collidingPairs;
		// update Rigidbodies
		for (auto &b : m_rigidBodies)
		{
			// Add drag
			b->acceleration += -b->velocity * 0.8f;
			b->accelerationAng += -b->velocityAng * 0.8f;

			b->Update(deltaTime);
			b->acceleration = { 0.0f, 0.0f };
		}
		// static collisions
		// dynamic collisions
			// calculate linear forces
			// calculate angular forces
		
		// reset accelerations
		for (auto &b : m_rigidBodies)
		{
			b->resetAcceleration();
		}
	}
	void GutPhysics2D::addRigidBody2D(glm::vec2 pos, float w, float h, float o)
	{
		m_rigidBodies.push_back(std::make_shared<Rectangle>(pos, w, h, o));
	}
	void GutPhysics2D::addRigidBody2D(const Rectangle& obj)
	{
		m_rigidBodies.push_back(std::make_shared<Rectangle>(obj));
	}
	
	void Rectangle::Update(float deltaTime)
	{
		// linear
		velocity += acceleration * deltaTime;
		position += velocity * deltaTime;
		// angular
		velocityAng += accelerationAng * deltaTime;
		orientation += velocityAng * deltaTime;
	}

	void Rectangle::DebugDraw(DebugRenderer & renderer)
	{
		glm::vec4 rect;
		rect.x = position.x;
		rect.y = position.y;
		rect.w = height;
		rect.z = width;
		renderer.drawBox(rect, Gutengine::ColorRGBA8(255, 0, 255, 255), orientation);
		renderer.drawCircle({ position.x, position.y }, Gutengine::ColorRGBA8(255, 0, 0, 255), 2.0f);
		// Corners
		renderer.drawCircle( getTLCorner(), Gutengine::ColorRGBA8(255, 0, 0, 255), 4.0f);
		renderer.drawCircle( getTRCorner(), Gutengine::ColorRGBA8(0, 255, 0, 255), 4.0f);
		renderer.drawCircle( getBRCorner(), Gutengine::ColorRGBA8(0, 0, 255, 255), 4.0f);
		renderer.drawCircle( getBLCorner(), Gutengine::ColorRGBA8(255, 255, 0, 255), 4.0f);
	}

	bool Rectangle::PointInShape(glm::vec2 point)
	{
		return false;
	}

	void Rectangle::ApplyLinearImpulse(glm::vec2 force)
	{
		// time is very small so we remove it from equation
		acceleration += force / mass;
	}

	void Rectangle::ApplyTorque(glm::vec2 force)
	{
	}

	void Rectangle::resetAcceleration()
	{
		acceleration = { 0.0f, 0.0f };
		accelerationAng = 0.0f;
	}

	AABB Rectangle::GetAABB()
	{
		AABB ab;
		if (orientation == 0.0f) // TODO: % 180.0f ?
		{
			ab.pos.x = position.x;
			ab.pos.y = position.y;
			ab.w = width;
			ab.h = height;
		}
		else
		{
			ab.w = fabs(height * sinf(orientation)) + fabs(width * cosf(orientation));
			ab.h = fabs(width * sinf(orientation))  + fabs(height * cosf(orientation));
			ab.pos.x = position.x;
			ab.pos.y = position.y;
		}

		return ab;
	}

	/* wh/hh = half width/height
	x-wh,y+hh ---- x+wh,y+hh
	|tl					tr|
	|					  |
	|bl					br|
	x-wh,y-hh ------ x+wh,y+hh
	*/

	glm::vec2 const Rectangle::getTLCorner() const
	{
		if (orientation == 0.0f) // TODO: % 180.0f ?
		{
			return{ position.x - width / 2.0f, position.y + height / 2.0f };
		}
		else
		{
			// point coords around origin
			glm::vec2 p = {-width / 2.0f, height / 2.0f };
			// point rotated
			glm::vec2 pr = { p.x * cosf(orientation) - p.y * sinf(orientation), p.x * sinf(orientation) + p.y * cosf(orientation) };
			return pr + position;
		}
	}

	glm::vec2 const Rectangle::getTRCorner() const
	{
		if (orientation == 0.0f) // TODO: % 180.0f ?
		{
			return{ position.x + width / 2.0f, position.y + height / 2.0f };
		}
		else
		{
			// point coords around origin
			glm::vec2 p = { width / 2.0f, height / 2.0f };
			// point rotated
			glm::vec2 pr = { p.x * cosf(orientation) - p.y * sinf(orientation), p.x * sinf(orientation) + p.y * cosf(orientation) };
			return pr + position;
		}
		
	};

	glm::vec2 const Rectangle::getBRCorner() const
	{
		if (orientation == 0.0f) // TODO: % 180.0f ?
		{
			return{ position.x + width / 2.0f, position.y - height / 2.0f };
		}
		else
		{
			// point coords around origin
			glm::vec2 p = { width / 2.0f, -height / 2.0f };
			// point rotated
			glm::vec2 pr = { p.x * cosf(orientation) - p.y * sinf(orientation), p.x * sinf(orientation) + p.y * cosf(orientation) };
			return pr + position;
		}
	};

	glm::vec2 const Rectangle::getBLCorner() const
	{
		if (orientation == 0.0f) // TODO: % 180.0f ?
		{
			return{ position.x - width / 2.0f, position.y - height / 2.0f };
		}
		else
		{
			// point coords around origin
			glm::vec2 p = { -width / 2.0f, -height / 2.0f };
			// point rotated
			glm::vec2 pr = { p.x * cosf(orientation) - p.y * sinf(orientation), p.x * sinf(orientation) + p.y * cosf(orientation) };
			return pr + position;
		}
	}

	glm::vec2 const Rectangle::getLinearVelocityOfPoint(const glm::vec2 point) const
	{
		glm::vec2 r = point - position;
		glm::vec2 rNormal = { -r.y, r.x };
		// Return velocity of corner with Chasles' Theorem
		return velocity + velocityAng * rNormal;
	};
}

/*
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
*/
