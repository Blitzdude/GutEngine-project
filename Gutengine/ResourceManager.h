#pragma once
#include "TextureCache.h"
#include <string>

namespace Gutengine 
{

//This is a way for us to access all our resources, such as
//Models or textures.
class ResourceManager
{
public:
    static GLTexture getTexture(std::string texturePath);

private:
    static TextureCache m_textureCache;
};

} // namespace end