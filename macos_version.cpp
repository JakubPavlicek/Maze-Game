#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <utility>
#include <map>
#include <fstream>
#include <sstream>

#include <ft2build.h>
#include FT_FREETYPE_H
//#include FT_GLYPH_H

#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define ASSERT(x) if (!(x)) __builtin_trap();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

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

static void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line )
{
    while(GLenum error = glGetError())
    {
        std::cout << "[OpenGL error] (" << error << "): \nFUNCTION: " << function << "\nFILE: " << file << " \nLINE: " << line << std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while(getline(stream, line))
    {
        if(line.find("#shader") != std::string::npos)
        {
            if(line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if(line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char message[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to coompile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader: " << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
}

int main()
{
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Maze Game", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
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
    
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
// END OF FREETYPE
    
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    const GLubyte* shading_language = glGetString(GL_SHADING_LANGUAGE_VERSION); // version as a string

    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    printf("Supported GLSL version is %s.\n", shading_language);
    //
    
    glfwSetKeyCallback(window, key_callback);
    
    float game_triangles[] = {
        -1.0f, -0.9f,
        -1.0f, -1.0f,
        -0.9f, -1.0f,
        -0.9f, -0.9f,
         1.0f,  0.9f,
         1.0f,  1.0f,
         0.9f,  1.0f,
         0.9f,  0.9f
    };
    
    unsigned int indices[] = {
        // player triangle
        0, 1, 2,
        2, 3, 0,
        // destination triangle
        4, 5, 6,
        6, 7, 4
    };
    
    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo));
    
    unsigned int vao1, vao2;
    GLCall(glGenVertexArrays(1, &vao1));
    GLCall(glGenVertexArrays(1, &vao2));

    unsigned int vbo;
    GLCall(glGenBuffers(1, &vbo));
    
    GLCall(glBindVertexArray(vao1));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 8 * 2 * sizeof(float), game_triangles, GL_STATIC_DRAW));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
//    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(4 * 2 * sizeof(float))));
    GLCall(glEnableVertexAttribArray(0));
//    GLCall(glEnableVertexAttribArray(1));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(unsigned int), indices, GL_STATIC_DRAW));
    
    GLCall(glBindVertexArray(vao2));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 8 * 2 * sizeof(float), game_triangles, GL_STATIC_DRAW));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(4 * 2 * sizeof(float))));
//    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(4 * 2 * sizeof(float))));
    GLCall(glEnableVertexAttribArray(0));
//    GLCall(glEnableVertexAttribArray(1));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(unsigned int), indices, GL_STATIC_DRAW));
    
    ShaderProgramSource source = ParseShader("OpenGL_tutorial/basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));
     
    int location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        
        glBindVertexArray(vao1);
        GLCall(glUniform4f(location, 0.0f, 1.0f, 0.0f, 1.0f));
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        glBindVertexArray(vao2);
        GLCall(glUniform4f(location, 1.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

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

    glDeleteProgram(shader);
    
    glfwTerminate();
    return 0;
}
