#pragma once
#include <glm\glm.hpp>
#include <Gutengine\Vertex.h>

class Light
{
public:
	Light();
	~Light();

	// Getters
	// Attributes

	// Methods 
	glm::vec2 _position;
	float _depth;
	float _range;
	float _size;
	Gutengine::ColorRGBA8 _color;
	float _intensity;

};

