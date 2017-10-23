#pragma once
#include <Box2D\Box2D.h>
class RayCastCallback :
	public b2RayCastCallback
{
public:
	RayCastCallback() {
		m_fixture = nullptr;
	}
	~RayCastCallback();

	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, float32 fraction)
	{

	}

	b2Fixture* m_fixture;
	b2Vec2 m_point;
	b2Vec2 m_normal;
	float m_fraction;



};

