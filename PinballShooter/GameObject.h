#pragma once

#include <Box2D\Box2D.h>
#include <glm\glm.hpp>
#include <Gutengine\Vertex.h>
#include <Gutengine\SpriteBatch.h>
#include <Gutengine\GLTexture.h>
#include <Gutengine\InputManager.h>
#include <Gutengine/Camera2D.h>
class GameObject
{
public:
	friend class Player;
	friend class Monster;
	friend class Pinball;

	GameObject();
	~GameObject();

	virtual void init(b2World* world,
		const glm::vec2 position,
		const glm::vec2 dimensions,
		Gutengine::GLTexture texture,
		Gutengine::ColorRGBA8 color,
		bool fixedRotation,
		b2BodyType bodyType,
		glm::vec4 uvRect) = 0;

	virtual void draw(Gutengine::SpriteBatch spriteBatch) = 0;
	virtual void update(Gutengine::InputManager& inputManager, Gutengine::Camera2D camera) = 0;

	// Getters
	b2Body* getBody() { return m_body; };
	b2Fixture* getFixture() { return m_fixture;	};
	glm::vec2 getDimensions() { return m_dimensions; };
	Gutengine::ColorRGBA8 getColor() { return m_color; };
	Gutengine::GLTexture getTexture() { return m_texture; };

protected:
	Gutengine::GLTexture m_texture;
	glm::vec2 m_dimensions;
	glm::vec4 m_uvRect;
	Gutengine::ColorRGBA8 m_color;
	b2Body* m_body;
	b2Fixture* m_fixture;
	

};

