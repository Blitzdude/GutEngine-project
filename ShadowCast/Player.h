#pragma once
#include "Box.h"
#include <Gutengine\SpriteBatch.h>
#include <Gutengine\GLTexture.h>
#include <Gutengine\InputManager.h>
#include <Gutengine\Camera2D.h>

const float PLAYER_MAX_SPEED = 7.0f;

class Player
{
public:
	Player();
	~Player();

	void init(b2World* world, const glm::vec2 position, const glm::vec2 dimensions, Gutengine::ColorRGBA8 color);

	void draw(Gutengine::SpriteBatch& spriteBatch);

	void update(Gutengine::InputManager& inputManager, Gutengine::Camera2D camera);

	//getters
	const Box& getBox() const { return m_collisionBox; };

private:
	Box m_collisionBox;
	//b2Fixture m_visionBox;
	glm::vec2 m_direction;
};

