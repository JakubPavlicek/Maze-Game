#include "Renderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H
//#include FT_GLYPH_H

#include <iostream>

void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while(GLenum error = glGetError())
    {
        std::cout << "[OpenGL error] (" << error << "): \nFUNCTION: " << function << "\nFILE: " << file << " \nLINE: " << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}

void Renderer::ClearWholeScreen() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawCell(float x, float y, const VertexArray& va, const IndexBuffer& ib, Shader& shader) const
{
    glm::vec3 translation(x, y, 0);
    glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(640), 0.0f, static_cast<float>(480));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 mvp = proj * view * model;
    shader.SetUniformMat4f("u_MVP", mvp);
    
    this->Draw(va, ib, shader);
 
}

void Renderer::DrawWalls(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    
    GLCall(glDrawArrays(GL_LINES, 0, ib.GetCount()));
}

int Renderer::InitializeFreetype()
{
    FT_Library ft;
    
    FT_Error error = FT_Init_FreeType( &ft );
    if ( error )
    {
        std::cout << "Error occured during FT initialisation" << std::endl;
        return -1;
    }
    
    FT_Face face;
    if (FT_New_Face(ft, "/System/Library/Fonts/Helvetica.ttc", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);
        
        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                         GL_TEXTURE_2D,
                         0,
                         GL_RED,
                         face->glyph->bitmap.width,
                         face->glyph->bitmap.rows,
                         0,
                         GL_RED,
                         GL_UNSIGNED_BYTE,
                         face->glyph->bitmap.buffer
                         );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    FT_Set_Pixel_Sizes(face, 0, 48);
    
    if(FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
        return -1;
    }
    
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return 0;
}

void Renderer::RenderText(std::string text, float x, float y, float scale)
{
    GLCall(glBindVertexArray(VAO));
    
    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];
        
        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        GLCall(glBindTexture(GL_TEXTURE_2D, ch.TextureID));
        // update content of VBO memory
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices)); // be sure to use glBufferSubData and not glBufferData
        
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        // render quad
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::RenderChar(char letter, float x, float y, float scale)
{
    GLCall(glBindVertexArray(VAO));
    
        Character ch = Characters[letter];
        
        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        GLCall(glBindTexture(GL_TEXTURE_2D, ch.TextureID));
        // set content of VBO memory
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
        
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        // render quad
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
