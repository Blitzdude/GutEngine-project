#pragma once
#include "Box.h"
#include <Gutengine\SpriteBatch.h>
#include <Gutengine\GLTexture.h>
class Player
{
public:
	Player();
	~Player();

	void init(b2World* world, const glm::vec2 position, const glm::vec2 dimensions,Gutengine::GLTexture texture, Gutengine::ColorRGBA8 color);

	void draw(Gutengine::SpriteBatch& spriteBatch);

	void update();

	//getters
	const Box& getBox() const { return m_collisionBox; };

private:
	Box m_collisionBox;
};

