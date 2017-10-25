#pragma once
#include <Box2D\Box2D.h>
#include <iostream>
// this class captures the closest hit shape : box2d.org/manual.pdf
class RayCastCallback :
	public b2RayCastCallback
{
public:
	RayCastCallback() {
		m_fixture = nullptr;
	}
	~RayCastCallback() {}

	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, float32 fraction) 
	{
		m_fixture = fixture;
		m_point = point;
		m_normal = normal;
		m_fraction = fraction;

		return m_fraction;
		
	}

	b2Fixture* m_fixture;
	b2Vec2 m_point;
	b2Vec2 m_normal;
	float m_fraction;

};

