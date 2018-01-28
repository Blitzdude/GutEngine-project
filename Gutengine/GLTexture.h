#pragma once
#include <GL/glew.h>
#include <string>

namespace Gutengine 
{

struct GLTexture 
{
    std::string filePath = "";
    GLuint id;
    int width;
    int height;
};

} // namespace end