#pragma once
#include <glm\glm.hpp>
#include "RayCastCallback.h"
#include <map>
#include <Box2D\Box2D.h>



class Vision
{
public:
	Vision();
	~Vision();

	void rayCastAndEmblace(b2Vec2 p1, b2Vec2 p2, b2World* world);
	float angleBetweenVecs(b2Vec2 p1, b2Vec2 p2);

	//members
	b2Vec2 reference;
	glm::vec2 point1;
	glm::vec2 point2;

	RayCastCallback rcCallback;
	std::map<float, glm::vec2> rayCastResults;
	float angle;
};

