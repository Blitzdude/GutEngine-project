#pragma once

#include <string>
#include <GL/glew.h>

namespace Gutengine 
{

//This class handles the compilation, linking, and usage of a GLSL shader program.
//Reference: http://www.opengl.org/wiki/Shader_Compilation
class GLSLProgram
{
public:
    GLSLProgram();
    ~GLSLProgram();

    void compileShaders			  (const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
    void compileShadersFromSource (const char* vertexSource, const char* fragmentSource);
    void addAttribute			  (const std::string& attributeName);
    GLint getUniformLocation	  (const std::string& uniformName);
    void linkShaders();
    void use();
    void unuse();
    void dispose();

private:
    void compileShader(const char* source, const std::string& name, GLuint id);
    
	int m_numAttributes;
    GLuint m_programID;
    GLuint m_vertexShaderID;
    GLuint m_fragmentShaderID;
};

} // namespace end
