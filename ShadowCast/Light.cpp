#pragma once
#include <glm\glm.hpp>
#include <Gutengine\Vertex.h>
#include <Gutengine\SpriteBatch.h>

class Light {
public:
	void draw(Gutengine::SpriteBatch& spriteBatch) {
		glm::vec4 destRect;	// create the destination rectangle
		destRect.x = position.x - size / 2.0f;
		destRect.y = position.y - size / 2.0f;
		destRect.z = size;
		destRect.w = size;
		spriteBatch.draw(destRect,	// Destination rectangle
						destRect,	// Uv-rectangle x,y,w,h
						0, 0.0f,	// texture to draw
						color,		// color
						0);			// rotation = 0
	};

	Gutengine::ColorRGBA8 color;
	glm::vec2 position;
	float size;
};