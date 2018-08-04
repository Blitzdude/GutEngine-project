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
	/*
void GutPhysics2D::updatePhysics()
{
	// container holding colliding pairs
	//std::vector<std::pair<RigidBody2D*, RigidBody2D*>> collidingPairs;
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
			//collidingPairs.push_back(std::make_pair(body, target));

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
		velocity += force / mass;
	}

	void Rectangle::ApplyTorque(glm::vec2 force)
	{
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
	};
}

/*
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
*/
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
