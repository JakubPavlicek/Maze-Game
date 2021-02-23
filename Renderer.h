#pragma once

#include <GL/glew.h>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Libraries.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#define ASSERT(x) if (!(x)) __builtin_trap();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

static std::map<GLchar, Character> Characters;
static unsigned int VAO, VBO;

class Renderer
{
public:
    void Clear() const;
    void ClearWholeScreen() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void DrawWalls(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
    void DrawCell(float x, float y, const VertexArray& va, const IndexBuffer& ib, Shader& shader) const;
    void SetLetter(float x, float y, glm::mat4 proj, glm::mat4 view, Shader& shader);
    void RenderText(std::string text, float x, float y, float scale);
    void RenderChar(char letter, float x, float y, float scale);
    int InitializeFreetype();
    void InitializeImGui();
};
