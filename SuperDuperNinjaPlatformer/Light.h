#pragma once

#include <glm/glm.hpp>
#include <Gutengine/Vertex.h>
#include <Gutengine/SpriteBatch.h>

class Light {
public:
    void draw(Gutengine::SpriteBatch& spriteBatch) {
        glm::vec4 destRect;
        destRect.x = position.x - size / 2.0f;
        destRect.y = position.y - size / 2.0f;
        destRect.z = size;
        destRect.w = size;
        spriteBatch.draw(destRect, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), 0, 0.0f, color, 0.0f);
    }

    Gutengine::ColorRGBA8 color;
    glm::vec2 position;
    float size;
};
