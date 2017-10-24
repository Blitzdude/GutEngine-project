#include "Vision.h"
#include <iostream>


static const float OFFSET = 0.0001f;
Vision::Vision()
{
	std::cout << "vision created" << std::endl;
}


Vision::~Vision()
{
	std::cout << "vision destroyed" << std::endl;
}


void Vision::rayCastAndEmblace(b2Vec2 p1, b2Vec2 p2, b2World* world)
{
	
	reference.x = p1.x + 1.0f;
	reference.y = p2.y;
	
	//float fraction = 0.0f;
	world->RayCast(&rcCallback, p1, p2);

	angle = angleBetweenVecs(p1, reference);
	
	rayCastResults.emplace(angle, glm::vec2(rcCallback.m_point.x, rcCallback.m_point.y));
}

float Vision::angleBetweenVecs(b2Vec2 p1, b2Vec2 p2)
{
	float a;
	float p1Length = p1.Length();
	float p2Length = p1.Length();

	a = acosf((p1.x * p2.x + p1.y * p2.x) / (p1Length * p2Length));
	return 1.0f;
}
