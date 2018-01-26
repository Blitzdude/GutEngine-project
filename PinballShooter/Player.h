#pragma once
#include "GameObject.h"
#include <Gutengine/SpriteBatch.h>
#include <Gutengine/Camera2D.h>
#include <Gutengine/InputManager.h>

class Player :
	public GameObject
{
public:
	Player();

	void init(b2World* world,
		const glm::vec2 position,
		const glm::vec2 dimensions,
		Gutengine::GLTexture texture,
		Gutengine::ColorRGBA8 color,
		bool fixedRotation,
		b2BodyType bodyType,
		glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)) override;

	void draw(Gutengine::SpriteBatch spriteBatch) override;
	void update(Gutengine::InputManager& inputManager, Gutengine::Camera2D camera) override;

	~Player();
private:
	glm::vec2 m_direction;
	
};

