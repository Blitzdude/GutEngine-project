#pragma once

#include "Capsule.h"
#include <Gutengine/InputManager.h>
#include <Gutengine/SpriteBatch.h>
#include <Gutengine/TileSheet.h>

enum class PlayerMoveState {STANDING, RUNNING, PUNCHING, IN_AIR };

class Player {
public:
    void init(b2World* world,
              const glm::vec2& position,
              const glm::vec2& drawDims,
              glm::vec2& collisionDims,
              Gutengine::ColorRGBA8 color);
    void destroy(b2World* world);

    void draw(Gutengine::SpriteBatch& spriteBatch);
    void drawDebug(Gutengine::DebugRenderer& debugRenderer);

    void update(Gutengine::InputManager& inputManager);

    const Capsule& getCapsule() const { return m_capsule; }

    glm::vec2 getPosition() const {
        glm::vec2 rv;
        rv.x = m_capsule.getBody()->GetPosition().x;
        rv.y = m_capsule.getBody()->GetPosition().y;
        return rv;
    }

    const glm::vec2& getDrawDims() const { return m_drawDims; }
    const glm::vec2& getCollisionDims() const { return m_collisionDims; }
    const Gutengine::ColorRGBA8& getColor() const { return m_color; }

private:
    glm::vec2 m_drawDims;
    glm::vec2 m_collisionDims;
    Gutengine::ColorRGBA8 m_color;
    Gutengine::TileSheet m_texture;
    Capsule m_capsule;
    PlayerMoveState m_moveState = PlayerMoveState::STANDING;
    float m_animTime = 0.0f;
    int m_direction = 1; // 1 or -1
    bool m_onGround = false;
    bool m_isPunching = false;
};

