#include "GutPhysics2D.h"
#include <iostream>
#include <algorithm>

namespace Gutengine
{
	Rectangle::Rectangle(glm::vec2 pos, float w, float h, float or/* = 0.0f */, float m /* = 1.0f */)
		: width(w), height(h)
	{
		mass = m;
		position.x = pos.x;
		position.y = pos.y;
		orientation = or;
		// J = 1/12 * mass * (w^2 + h^2);
		angularMass = 0.08333333f * mass * (width*width + height * height);
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
		// container holding colliding pairs TODO: when colliding dynamically
		//std::vector<std::pair<std::shared_ptr<RigidBody>, std::shared_ptr<RigidBody> > > collidingPairs;
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
		for (auto &itr = m_rigidBodies.begin(); std::next(itr) != m_rigidBodies.end(); ++itr)
		{
			for (auto &itr_n = std::next(itr); itr_n != m_rigidBodies.end(); ++itr_n)
			{
				auto current = (*itr)->GetAABB();
				auto other = (*itr_n)->GetAABB();
				// Do they overlap
				if (current.overlaps(other))
				{
					// SAT collision
					glm::vec2 doNothing;
					//std::dynamic_pointer_cast<Rectangle>((itr->get()));
					// TODO: HOW TO CAST !!!!

					// get normals to check
					auto n1 = (*itr)->getUniqueNormals();
					auto n2 = (*itr_n)->getUniqueNormals();

					std::vector<glm::vec2> normals;

					normals.insert(std::end(normals), std::begin(n1), std::end(n1) );
					normals.insert(std::end(normals), std::begin(n2), std::end(n2) );

					// project to normals
					for (auto n : normals)
					{

					}
					// check overlap

					// calculate minimum translation vector - MTV
					
					float left =    (other.pos.x - other.w / 2.0f) - (current.pos.x + current.w / 2.0f) ;
					float right =   (other.pos.x + other.w / 2.0f) - (current.pos.x - current.w / 2.0f) ;
					float top =     (other.pos.y - other.h / 2.0f) - (current.pos.y + current.h / 2.0f) ;
					float bottom =  (other.pos.y + other.h / 2.0f) - (current.pos.y - current.h / 2.0f) ;
					
					glm::vec2 mtv;
					
					if (fabs(left) > fabs(right))
						mtv.x = right;
					else
						mtv.x = left;
					
					if (fabs(bottom) > fabsf(top))
						mtv.y = top;
					else
						mtv.y = bottom;

					if (fabs(mtv.x) <= fabs(mtv.y))
						mtv.y = 0.0f;
					else
						mtv.x = 0.0f;
					
					// now we have minimum translation, move each object with it
					(*itr)->position += mtv;
					(*itr_n)->position += -mtv;
				}
			}
		}
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

	/* Returns the minimum and maximum values of the projection on vec2 form (x = min, y = max) */
	glm::vec2 GutPhysics2D::projectShapeToAxis(const Rectangle & shape, const glm::vec2 & axis) const
	{
		glm::vec2 axisN = axis;
		// NOTE: axis should be normalized
		if (axisN.length() > 1.0f)
			axisN = glm::normalize(axisN);

		glm::vec2 minMax;
		minMax.x = glm::dot(axisN, shape.getTLCorner());
		minMax.y = minMax.x;
		for (auto itr : shape.getCorners()) // currently some redundacy, with checkin the first element twice
		{
			float p = glm::dot(axisN, itr);
			if (p < minMax.x) // less then minimum
				minMax.x = p;
			else if (p > minMax.y) // greater then maximum
				minMax.y = p;
		}

		return minMax;
	}
	/*
	glm::vec2 GutPhysics2D::vectorProjectToAxis(const glm::vec2 & vec, const glm::vec2 & axis) const
	{
		// normal of axis
		glm::vec2 axisN = glm::normalize(axis);
		// dot product
		float dot = glm::dot(vec, axisN);
		return axisN * dot;
	}
	*/

	/* Returns true if overlapping, and returns the reference of minMax values */
	bool GutPhysics2D::checkSatCollision(const Rectangle & a, const Rectangle & b, glm::vec2 & minMax)
	{
		// loop over get Axises
		// get normals to check
		auto n1 = a.getUniqueNormals();
		auto n2 = b.getUniqueNormals();

		std::vector<glm::vec2> normals;

		normals.insert(std::end(normals), std::begin(n1), std::end(n1));
		normals.insert(std::end(normals), std::begin(n2), std::end(n2));

		// project to normals
		for (auto n : normals)
		{
			// 
			glm::vec2 projA = projectShapeToAxis(a, n);
			glm::vec2 projB = projectShapeToAxis(b, n);

			// if projections do not overlap -> break true
			if (!(projA.x <= projB.y && projA.y >= projB.x))
			{
				return true;
			}
		}
		// if no overlaps were found: return false
		// TODO: calculate Minimum translation vector
		minMax = glm::vec2(0.0f, 0.0f); // return zero vector for now
		return false;
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
		// normals
		for (auto n : getUniqueNormals())
		{
			renderer.drawLine(position, position + (50.0f * n), Gutengine::ColorRGBA8(0, 0, 255, 255));
		}
	}

	bool Rectangle::PointInShape(glm::vec2 point)
	{
		return false;
	}

	void Rectangle::ApplyLinearForce(glm::vec2 force)
	{
		// time is very small so we remove it from equation
		acceleration += force / mass;
	}

	void Rectangle::ApplyTorque(float force)
	{
		accelerationAng += force / angularMass;
	}

	void Rectangle::ApplyTorqueToPoint(glm::vec2 point, glm::vec2 force)
	{
		// vector from CM -> point
		glm::vec2 r = point - position;
		// normal of r
		glm::vec2 rNormal = { -r.y, r.x };
		// Torque relative from point equals the dot product
		float torque = glm::dot(rNormal, force);
		// finally apply torque relative to center
		ApplyTorque(torque);
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

	std::vector<glm::vec2> Rectangle::getCorners() const
	{
		std::vector<glm::vec2> ret;

		ret.push_back(getTLCorner());
		ret.push_back(getTRCorner());
		ret.push_back(getBRCorner());
		ret.push_back(getBLCorner());

		return ret;
	}

	std::vector<glm::vec2> Rectangle::getUniqueNormals() const
	{
		std::vector<glm::vec2> ret;
		
		// get vectors from corner to the next
		ret.push_back(getTRCorner() - getTLCorner());
		ret.push_back(getBRCorner() - getTRCorner());
		// make normals and normalize them
		for (auto itr = ret.begin(); itr != ret.end(); ++itr)
		{
			glm::vec2 temp = glm::normalize(glm::vec2(-itr->y, itr->x));
			*itr = temp;
		}

		return ret;
	}

	glm::vec2 const Rectangle::getLinearVelocityOfPoint(const glm::vec2 point) const
	{
		glm::vec2 r = point - position;
		glm::vec2 rNormal = { -r.y, r.x };
		// Return velocity of corner according to  Chasles' Theorem
		return velocity + velocityAng * rNormal;
	}
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
