#pragma once
#include <Box2D\Box2D.h>
// this class captures the closest hit shape : box2d.org/manual.pdf
class AABBQueryCallback :
	public b2QueryCallback
{
public:
	AABBQueryCallback() 
	{
	}

	~AABBQueryCallback() 
	{
	}

	bool ReportFixture(b2Fixture* fixture) 
	{
		b2Body* body = fixture->GetBody();
		body->SetAwake(true);

		// return true to continue the query
		return true;
	}
};

