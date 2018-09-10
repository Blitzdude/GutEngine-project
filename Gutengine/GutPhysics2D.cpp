#include "GutPhysics2D.h"
#include <iostream>
#include <algorithm>

namespace Gutengine
{
	unsigned int RigidBody::objectCount = 0;

	Rectangle::Rectangle(glm::vec2 pos, float w, float h, float or/* = 0.0f */, float m /* = 1.0f */)
		: width(w), height(h)
	{
		id = objectCount++;
		mass = m;
		position.x = pos.x;
		position.y = pos.y;
		orientation = or;
		// J = mass * (w^2 + h^2);
		angularMass = mass * width * height;
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
		// CALCULATE FORCES AND TORQUES
		computeForces(deltaTime);
		for (auto &b : m_rigidBodies)
		{
			if (!b->getStatic())
			{
				// Add drag
				b->acceleration += b->velocity * 0.8f;
				b->accelerationAng += b->velocityAng * 0.8f;
				// gravity
				b->acceleration += m_gravity;
			}
		}

		// COLLISION CHECKING
		checkCollisions();
		
		// COLLISION RESOLUTION
		resolveCollisions();
		
		// UPDATE OBJECT VELOCITY AND POSITION
		for (auto &b : m_rigidBodies)
		{
			b->Update(deltaTime);
		}
		// CLEAR ACCELERATIONS AND VECTORS
	}
	std::shared_ptr<Gutengine::Rectangle> GutPhysics2D::addRigidBody2D(glm::vec2 pos, float w, float h, float o)
	{
		m_rigidBodies.push_back(std::make_shared<Rectangle>(pos, w, h, o));
		return m_rigidBodies.back();
	}
	std::shared_ptr<Gutengine::Rectangle> GutPhysics2D::addRigidBody2D(const Rectangle& obj)
	{
		m_rigidBodies.push_back(std::make_shared<Rectangle>(obj));
		return m_rigidBodies.back();
	}

	/* Returns the minimum and maximum values of the projection on vec2 form (x = min, y = max) */
	glm::vec2 GutPhysics2D::projectShapeToAxis(const Rectangle & shape, const glm::vec2 & axis) const
	{
		glm::vec2 axisN = glm::normalize(axis);
		// NOTE: axis should be normalized

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

	/* Returns true if overlapping, and returns the reference of minMax values */
	bool GutPhysics2D::checkSatCollision(Rectangle & a, Rectangle & b, SatManifold& mtv)
	{
		// get normals to check
		auto n1 = a.getUniqueNormals();
		auto n2 = b.getUniqueNormals();

		std::vector<glm::vec2> normals;
		normals.insert(std::end(normals), std::begin(n1), std::end(n1));
		normals.insert(std::end(normals), std::begin(n2), std::end(n2));

		glm::vec2 smallestAxis = normals[0];
		float smallestLength = INFINITY;
		// project to normals
		for (auto n : normals)
		{
			// Projection minMax, x = min, y = max
			glm::vec2 projA = projectShapeToAxis(a, n);
			glm::vec2 projB = projectShapeToAxis(b, n);

			/* if projections don't overlap 
			   there is a gap, and no possibility of collision */
			if (projB.x >= projA.y || projA.x >= projB.y )
			{
				mtv.axis = glm::vec2(0.0f, 0.0f); // return zero
				mtv.contactPoint = glm::vec2(0.0f, 0.0f); // return zero
				mtv.length = 0.0f;
				mtv.left = nullptr;
				mtv.right = nullptr;

				return false;
			}
			else
			{
				// get the amount of overlap
				float o;
				float o1 = projA.y - projB.x;
				float o2 = projB.y - projA.x;
				if (fabs(o1) < fabs(o2))
					o = fabs(o1);
				else
					o = fabs(o2);

				// if amount of overlaps is smaller then current amount
				if (o < smallestLength)
				{
					smallestLength = o;
					smallestAxis = n;
				}			
			}
		}
		mtv.left = &a; // asume that a penetrates b;
		mtv.right = &b;
		mtv.length = smallestLength;
		mtv.axis = smallestAxis;
		// if no gaps were found: rects overlap;
		// find the contact point and determine if a is penetrating b or vice versa.
		bool penetrationFound = false;
		for (auto corner : a.getCorners())
		{
			if (b.PointInShape(corner))
			{
				// a is penetrating b
				mtv.contactPoint = corner;
				// get the edge normal pointing away from b
				mtv.edge = b.getClosestEdge(corner);
				// make it perpendicular
				glm::vec2 vecN = (mtv.edge.b - mtv.edge.a);
				mtv.normal = glm::normalize(glm::vec2( -vecN.y, vecN.x ) );
				penetrationFound = true;
				break;
			}
		}
		if (penetrationFound == false)
		{
			// check if b penetrates a
			for (auto corner : b.getCorners())
			{
				if (a.PointInShape(corner))
				{
					mtv.left = &b;
					mtv.right = &a;
					// b is penetrating a
					mtv.contactPoint = corner;
					// get the closest edge
					mtv.edge = a.getClosestEdge(corner);
					// Create edge normal away from b
					glm::vec2 vecN = (mtv.edge.b - mtv.edge.a);
					mtv.normal = glm::normalize(glm::vec2(-vecN.y, vecN.x));
					penetrationFound = true;
					break;
				}
			}
		}

		return true;
	}

	void GutPhysics2D::resolveCollisions()
	{
		if (!vecManifolds.empty())
		{
			for (auto itr : vecManifolds)
			{
				// STATIC RESOLUTION
				// Check which side to push objects with dot product
				if (glm::dot(itr.left->position - itr.right->position, itr.axis) < 0.0f)
				{
					if (itr.right->getStatic())
					{
						itr.left->position += -itr.axis * (itr.length);

					}
					else if (itr.left->getStatic())
					{
						itr.right->position += itr.axis * (itr.length);
					}
					else
					{
						itr.left->position += -itr.axis * (itr.length / 2.0f);
						itr.right->position += itr.axis * (itr.length / 2.0f);
					}

				}
				else
				{
					if (itr.right->getStatic())
					{
						itr.left->position += itr.axis * (itr.length);
					}
					else if (itr.left->getStatic())
					{
						itr.right->position += -itr.axis * (itr.length);
					}
					else
					{
						itr.left->position += itr.axis * (itr.length / 2.0f);
						itr.right->position += -itr.axis * (itr.length / 2.0f);
					}

				}

				// DYNAMIC RESOLUTION
				// A IS STATIC
				if (itr.left->getStatic()) 
				{
					auto a = itr.left; 
					auto b = itr.right;
					auto p = itr.contactPoint;

					// normal is the normalized vector perpendicular to the edge beign interpenetrated.
					// pointing away from b.
					glm::vec2 normal = itr.normal;

					glm::vec2 r_ap = p - a->position;
					glm::vec2 rp_ap = { -r_ap.y, r_ap.x }; // perpendicular
					glm::vec2 r_bp = p - b->position;
					glm::vec2 rp_bp = { -r_bp.y, r_bp.x }; // perpendicular

					glm::vec2 v_ap = a->getLinearVelocityOfPoint(p);
					glm::vec2 v_bp = b->getLinearVelocityOfPoint(p);

					glm::vec2 v_ab = -v_bp;

					// if relative normal velocity (v_ab dot normal) is < 0 -> rects are colliding
					if (glm::dot(v_ab, normal) >= 0.0f)
						continue; // a and b not colliding, but moving away from each other.

					// calculate j (a is static)
					float j_numerator = -(1 + e) * glm::dot(v_ab, normal);
					float j_denominator =
						(1.0f / b->mass)
						+ (glm::dot(rp_bp, normal) * glm::dot(rp_bp, normal)) / b->angularMass;

					float j = j_numerator / j_denominator;

					itr.right->velocity -= (j * normal) / itr.right->mass;
					itr.right->velocityAng -= glm::dot(rp_bp, j*normal) / itr.right->angularMass;

				}
				// B IS STATIC
				else if (itr.right->getStatic())
				{
					auto a = itr.left; // a penetrates b
					auto b = itr.right;
					auto p = itr.contactPoint;

					// normal is the normalized vector perpendicular to the edge beign interpenetrated.
					// pointing away from b.
					glm::vec2 normal = itr.normal;

					glm::vec2 r_ap = p - a->position;
					glm::vec2 rp_ap = { -r_ap.y, r_ap.x }; // perpendicular

					glm::vec2 v_ap = a->getLinearVelocityOfPoint(p);

					glm::vec2 v_ab = v_ap; // assume that static object doesn't have velocity

					// if relative normal velocity (v_ab dot normal) is < 0 -> rects are colliding
					if (glm::dot(v_ab, normal) >= 0.0f)
						continue; // a and b not colliding, but moving away from each other.

					// calculate j ( b is static)
					float j_numerator = -(1 + e) * glm::dot(v_ab, normal);
					float j_denominator =
						(1.0f / a->mass)
						+ (glm::dot(rp_ap, normal) * glm::dot(rp_ap, normal)) / a->angularMass;

					float j = j_numerator / j_denominator;

					itr.left->velocity += (j * normal) / itr.left->mass;
					itr.left->velocityAng += glm::dot(rp_ap, j*normal) / itr.left->angularMass;

					//itr.left->velocity += (j * normal) / itr.left->mass;
					//itr.left->velocityAng += glm::dot(rp_ap, j*normal) / itr.left->angularMass;
				}
				else // NEITHER IS STATIC 
				{
					auto a = itr.left; // a penetrates b
					auto b = itr.right;
					auto p = itr.contactPoint;

					// normal is the normalized vector perpendicular to the edge beign interpenetrated.
					// pointing away from b.
					glm::vec2 normal = itr.normal;

					glm::vec2 r_ap = p - a->position;
					glm::vec2 rp_ap = { -r_ap.y, r_ap.x }; // perpendicular
					glm::vec2 r_bp = p - b->position;
					glm::vec2 rp_bp = { -r_bp.y, r_bp.x }; // perpendicular

					glm::vec2 v_ap = a->getLinearVelocityOfPoint(p);
					glm::vec2 v_bp = b->getLinearVelocityOfPoint(p);

					glm::vec2 v_ab = v_ap - v_bp;

					// if relative normal velocity (v_ab dot normal) is < 0 -> rects are colliding
					if (glm::dot(v_ab, normal) >= 0.0f)
						continue; // a and b not colliding, but moving away from each other.

					// calculate j
					float j_numerator = -(1 + e) * glm::dot(v_ab, normal);
					float j_denominator =
						(1.0f / a->mass) + (1.0f / b->mass)
						+ (glm::dot(rp_ap, normal) * glm::dot(rp_ap, normal)) / a->angularMass
						+ (glm::dot(rp_bp, normal) * glm::dot(rp_bp, normal)) / b->angularMass;

					float j = j_numerator / j_denominator;

					itr.left->velocity +=  (j * normal) / itr.left->mass;
					itr.left->velocityAng += glm::dot(rp_ap, j*normal) / itr.left->angularMass;

					itr.right->velocity -= (j * normal) / itr.right->mass;
					itr.right->velocityAng -= glm::dot(rp_bp, j*normal) / itr.right->angularMass;
				}

			}
		}
	}

	void GutPhysics2D::computeForces(float deltaTime)
	{
		// TODO: Is static checking
		for (auto itr : m_rigidBodies)
		{
			if (!itr->getStatic())
			{
				// apply gravity
				itr->acceleration += m_gravity / itr->mass;
				// apply damping
				itr->acceleration -= 0.8f * itr->velocity;
				itr->accelerationAng -= 0.8f * itr->velocityAng;
				// integrate accelration and velocity
				itr->Update(deltaTime);
			}
		}
	}

	void GutPhysics2D::checkCollisions()
	{
		for (auto &itr = m_rigidBodies.begin(); std::next(itr) != m_rigidBodies.end(); ++itr)
		{
			// COLLISION CHECKING
			for (auto &itr_n = m_rigidBodies.begin(); itr_n != m_rigidBodies.end(); ++itr_n)
			{
				//filter out self self checking
				if (itr->get()->id == itr_n->get()->id)
					continue;

				auto current = (*itr)->GetAABB();
				auto other = (*itr_n)->GetAABB();
				// AABB Do they overlap
				if (current.overlaps(other))
				{
					// SAT collision
					SatManifold mtv;
					//std::dynamic_pointer_cast<Rectangle>((itr->get()));
					// TODO: HOW TO CAST !!!!
					if (checkSatCollision(*(*itr), *(*itr_n), mtv))
					{
						// calculate minimum translation vector - MTV
						// now we have minimum translation, move each object with it
						vecManifolds.push_back(mtv);
					}
				}
			}
		}
	}

	void GutPhysics2D::clear()
	{
		for (auto b : m_rigidBodies)
		{
			b->resetAcceleration();
		}
		vecManifolds.clear();
	}
	
	void Rectangle::Update(float deltaTime)
	{
		// velocity
		if (this->getStatic())
		{
			velocity = { 0.0f, 0.0f };
			velocityAng = 0.0f;
		}
		else
		{
			velocity += acceleration * deltaTime;
			velocityAng += accelerationAng * deltaTime;
			// position - with damping
			if (glm::length(velocity) > 0.1f)
				position += velocity * deltaTime;
			if (fabs(velocityAng) > 0.5f);
				orientation += velocityAng * deltaTime;
		}
	}

	bool Rectangle::PointInShape(glm::vec2 point)
	{
		// for triangle in point detection
		auto sameSide = [&](glm::vec2 p1, glm::vec2 p2, glm::vec2 a, glm::vec2 b) -> bool 
		{
			glm::vec3 p1_3 = { p1.x, p1.y, 0.0f };
			glm::vec3 p2_3 = { p2.x, p2.y, 0.0f };
			glm::vec3 a_3 = { a.x, a.y, 0.0f };
			glm::vec3 b_3 = { b.x, b.y, 0.0f };

			glm::vec3 cp1 = glm::cross(b_3 - a_3, p1_3 - a_3);
			glm::vec3 cp2 = glm::cross(b_3 - a_3, p2_3 - a_3);

			if (glm::dot(cp1, cp2) >= 0.0f)
				return true;
			else
				return false;
		};

		auto pointInTringle = [&](glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c) -> bool
		{
			return (sameSide(p, a, b, c) && sameSide(p, b, a, c) && sameSide(p, c, a, b));
		};

		return (pointInTringle(point, getTLCorner(), getTRCorner(), getBRCorner() ) ||
				pointInTringle(point, getBRCorner(), getBLCorner(), getTLCorner() ) );
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
	x-wh,y-hh ---- x+wh,y+hh
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
		std::vector<glm::vec2> sides;
		std::vector<glm::vec2> ret;
		
		// get vectors from corner to the next
		sides.push_back(getTRCorner() - getTLCorner());
		sides.push_back(getBRCorner() - getTRCorner());
		// make normals and normalize them
		for (auto itr = sides.begin(); itr != sides.end(); ++itr)
		{
			ret.push_back(glm::normalize(glm::vec2(itr->y, -itr->x)) );
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
	std::vector<Edge> Rectangle::getEdges() const
	{
		/*
		TL --> TR
		 ^		|
		 |		v
		BL <-- BL
		*/
		std::vector<Edge> ret;
		ret.push_back({getTLCorner(), getTRCorner()}); // 1
		ret.push_back({getTRCorner(), getBRCorner()}); // 2
		ret.push_back({getBRCorner(), getBLCorner()}); // 3
		ret.push_back({getBLCorner(), getTLCorner()}); // 4

		return ret;
	}
	Edge Rectangle::getClosestEdge(glm::vec2 point) const
	{
		auto edges = getEdges();

		float smallest = INFINITY; // Huge number to start with
		Edge ret;
		// test distance from point to all edges
		for (auto e : edges)
		{
			glm::vec2 p = e.b - e.a;
			glm::vec2 perp = { -p.y, p.x }; // perpendicular to edge
			float pseudDist = std::fabsf(glm::dot(point - e.a, perp)); // pseudodistance from point to edge
			if (smallest > pseudDist)
			{
				smallest = pseudDist;
				ret = e;
			}
		}
		// return the vector of the edge
		return ret;
	}
} // Namespace end
