#pragma once
#include <GL/glew.h>
#include "GLTexture.h"

#include <string>

namespace Gutengine 
{

//A 2D quad that can be rendered to the screen
class Sprite
{
public:
    Sprite();
    ~Sprite();

    void init(float x, float y, float width, float height, std::string texturePath);

    void draw();

    //private:
    float m_x;
    float m_y;
    float m_width;
    float m_height;
    GLuint m_vboID;
    GLTexture m_texture;

};

} // namespace end