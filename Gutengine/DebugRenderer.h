#pragma once
#include "GLSLProgram.h"
#include "Vertex.h"
#include <glm/glm.hpp>
#include <vector>
#include <map>

namespace Gutengine { // TODO: draw polygon
    class DebugRenderer {
    public:
        DebugRenderer();
        ~DebugRenderer();

        void init();
        void end();
        void drawLine(const glm::vec2& a, const glm::vec2& b, const ColorRGBA8& color);
        void drawBox(const glm::vec4& destRect, const ColorRGBA8& color, float angle);
        void drawCircle(const glm::vec2& center, const ColorRGBA8& color, float radius);
		void drawPolygon(const std::vector<glm::vec2> & vertices, const ColorRGBA8& color, const GLint numVertices);
        void render(const glm::mat4& projectionMatrix, float lineWidth);
        void dispose();

        struct DebugVertex {
            glm::vec2 position;
            Gutengine::ColorRGBA8 color;
        };
    private:
        Gutengine::GLSLProgram m_program;
        std::vector<DebugVertex> m_verts;
        std::vector<GLuint> m_indices;
        GLuint m_vbo = 0, m_vao = 0, m_ibo = 0; // ibo = index buffer object
        int m_numElements = 0;
    };

}