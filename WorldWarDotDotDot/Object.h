#pragma once
#include <glm/glm.hpp>
#include <Box2D/Box2D.h>
#include <Gutengine/GLTexture.h>
#include <Gutengine/SpriteBatch.h>
#include "Grid.h"

class Object
{
public:
	Object();
	~Object();

	void virtual init(b2World* world,
        const glm::vec2 position,
        const float width,
        const float height,
        Gutengine::GLTexture texture,
        Gutengine::ColorRGBA8 color,
        bool fixedRotation,
        b2BodyType bodyType,
        float density,
        float friction,
        glm::vec4 uvRect) = 0;



	void virtual destroy(b2World &world);
	void virtual draw(Gutengine::SpriteBatch &spriteBatch) = 0;
	void virtual update(Grid &grid) = 0;

    // GETTERS
    virtual b2Body* getBody() const { return m_body; };
    virtual b2Fixture* getFixture() const { return m_fixture; };
    virtual const Gutengine::ColorRGBA8& getColor() const { return m_color; };

    // SETTERS
    virtual void setColor(const Gutengine::ColorRGBA8 newColor) { m_color = newColor; };


private:
	// box2D
	b2Body* m_body;
	b2Fixture* m_fixture;
	glm::vec2 m_position;
	float m_orientation;
	float m_radius;
    Gutengine::ColorRGBA8 m_color;

	float m_speed = 500.0f;

	Gutengine::GLTexture m_texture;

	bool isDynamic = true;
	bool hasFixedRotation = true;
};

