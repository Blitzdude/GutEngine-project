#pragma once
#include "GameEntity.h"
#include "Circle.h"
#include <Gutengine\SpriteBatch.h>
#include <Gutengine\GLTexture.h>
#include <Gutengine\InputManager.h>
#include <Gutengine\Camera2D.h>


const float PINBALL_MAX_SPEED = 10.0f;
const float PINBALL_SPEED = 5.0f;

class Pinball : public GameEntity
{
public:
	Pinball();
	~Pinball();

	void init(b2World* world, const glm::vec2 position, const float radius, Gutengine::ColorRGBA8 color);

	void draw(Gutengine::SpriteBatch& spriteBatch);

	void update(Gutengine::InputManager& inputManager, Gutengine::Camera2D camera);

	// collision userdata functions
	void startContact();
	void endContact();


	//getters
	EntityType getEntityType() override { return EntityType::ET_PINBALL; };

	const Circle& getCircle() const { return m_collisionCircle; };
	const glm::vec2 getPosition() const;

private:
	int m_contacts;
	Circle m_collisionCircle;
	glm::vec2 m_direction;
};

