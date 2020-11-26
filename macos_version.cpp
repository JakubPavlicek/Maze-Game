#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdio>
#include <utility>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H
//#include FT_GLYPH_H

#include "glm.hpp"

static float posX = 0, posY = 0;

static int WIDTH = 640, HEIGHT = 480;

std::pair<int, int> DisableWindowAwayPos()
{
    if(posX < 0)
        posX = 0;
    if(posX > 1.9)
        posX = 1.9;
    if(posY < 0)
        posY = 0;
    if(posY > 1.9)
        posY = 1.9;

    return { posX, posY };
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    DisableWindowAwayPos();

    switch(key)
    {
    default:
        if(action == GLFW_PRESS || action == GLFW_REPEAT)
            std::cout << "spatne tlacitko" << std::endl;
        break;

    case GLFW_KEY_ESCAPE:
            exit(0);
            
    case GLFW_KEY_UP:
        switch(action)
        {
            case GLFW_PRESS:
                std::cout << "Nahoru" << std::endl;
                posY += 0.05f;
                break;
            case GLFW_REPEAT:
                std::cout << "Nahoru - opak" << std::endl;
                posY += 0.05f;
                break;
        }
        break;

    case GLFW_KEY_DOWN:
        switch(action)
        {
            case GLFW_PRESS:
                std::cout << "Dolu" << std::endl;
                posY -= 0.05f;
                break;
            case GLFW_REPEAT:
                std::cout << "Dolu - opak" << std::endl;
                posY -= 0.05f;
                break;
        }
        break;
            
    case GLFW_KEY_LEFT:
        switch(action)
        {
            case GLFW_PRESS:
                std::cout << "Doleva" << std::endl;
                posX -= 0.05f;
                break;
            case GLFW_REPEAT:
                std::cout << "Doleva - opak" << std::endl;
                posX -=  0.05f;
                break;
        }
        break;
            
    case GLFW_KEY_RIGHT:
        switch(action)
        {
            case GLFW_PRESS:
                std::cout << "Doprava" << std::endl;
                posX += 0.05f;
                break;
            case GLFW_REPEAT:
                std::cout << "Doprava - opak" << std::endl;
                posX += 0.05f;
                break;
        }
    break;
    }
}

struct Character
{
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size;        // Size of glyph
    glm::ivec2 Bearing;     // Offset from baseline to left/top glyph
    unsigned int Advance;   // Offset to advance to next glyph
};

int main()
{
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Maze Game", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    glewInit();

// FREETYPE
    FT_Library ft;

    FT_Error error = FT_Init_FreeType( &ft );
    if ( error )
    {
        std::cout << "Error occured during FT initialisation" << std::endl;
    }
    
    FT_Face face;
    if (FT_New_Face(ft, "/System/Library/Fonts/Helvetica.ttc", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    
    FT_Set_Pixel_Sizes(face, 0, 48);
    
    if(FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
        return -1;
    }
    
    std::map<char, Character> Characters;
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    
    for(unsigned int i = 0; i < 128; i++)
    {
        // load character glyph
        if(FT_Load_Char(face, i, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
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
            (unsigned int)face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(i, character));
    }
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
// END OF FREETYPE
    
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    
    glfwSetKeyCallback(window, key_callback);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.3f, 0.3f, 0.3f, 1);
        
        // player's GL_QUADS
        glColor3f(0, 0.8, 0);
        glBegin(GL_QUADS);
        glVertex2f(-1.0f + posX, -0.9f + posY);
        glVertex2f(-0.9f + posX, -0.9f + posY);
        glVertex2f(-0.9f + posX, -1.0f + posY);
        glVertex2f(-1.0f + posX, -1.0f + posY);
        glEnd();
        
        // end's GL_QUADS
        glColor3f(1, 0, 0);
        glBegin(GL_QUADS);
        glVertex2f(0.9f, 1.0f);
        glVertex2f(1.0f, 1.0f);
        glVertex2f(1.0f, 0.9f);
        glVertex2f(0.9f, 0.9f);
        glEnd();
        
        // walls
        glColor3f(1,1,1);
//        glLineWidth(5);
//        glBegin(GL_LINES);
//        glVertex2f(-0.2, 0);
//        glVertex2f(-0.2, 1);
//        glEnd();
//
//        glLineWidth(5);
//        glBegin(GL_LINES);
//        glVertex2f(-0.6, 0);
//        glVertex2f(-0.6, 1);
//        glEnd();

        glLineWidth(5);
        glBegin(GL_LINES);
        glVertex2f(0, -0.5);
        glVertex2f(1, -0.5);
        glEnd();
        if(posX >= 0.95f && posX <= 1.95f && posY >= 0.45f && posY <= 0.5f)
            std::cout << "PROHRAL SI" << std::endl;
        
        
        glLineWidth(5);
        glBegin(GL_LINES);
        glVertex2f(-1, -0.3);
        glVertex2f(0, -0.3);
        glEnd();
        if(posX >= -0.1f && posX <= 1.0f && posY >= 0.65f && posY <= 0.7f)
            std::cout << "PROHRAL SI" << std::endl;
        
        // Win
        if(posX >= 1.8f && posY >= 1.8f)
        {
            bool erase_loop = false;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            erase_loop = true;
            while(erase_loop)
            {
                posX = 2;
                posY = 2;
                erase_loop = false;
            }
            // tady napsat "YOU WIN"
        }
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    
    return 0;
}
