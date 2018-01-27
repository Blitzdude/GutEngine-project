#pragma once
#include "Circle.h"
#include "GameEntity.h"
#include <Gutengine\SpriteBatch.h>
#include <Gutengine\GLTexture.h>
#include <Gutengine\InputManager.h>
#include <Gutengine\Camera2D.h>

const float ENEMY_MAX_SPEED = 5.0f;
const float ENEMY_SPEED = 3.0f;

class Enemy : public GameEntity
{
public:
	Enemy();
	~Enemy();

	void init(b2World* world, const glm::vec2 position, const float radius, Gutengine::ColorRGBA8 color);

	void draw(Gutengine::SpriteBatch& spriteBatch);

	void update(Gutengine::InputManager& inputManager, Gutengine::Camera2D camera, glm::vec2 playerPosition);

	EntityType getEntityType() override { return EntityType::ET_ENEMY; };

	//getters
	const Circle& getCircle() const { return m_collisionCircle; };
	const glm::vec2 getPosition() const;

private:
	Circle m_collisionCircle;
	glm::vec2 m_direction;
};

