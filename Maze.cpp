#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <utility>
#include <map>
#include <sstream>
#include <stack>

#include <ft2build.h>
#include FT_FREETYPE_H
//#include FT_GLYPH_H

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "Maze.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

static float posX = -1.0f, posY = -1.0f;

static int WIDTH = 640, HEIGHT = 480;

void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);

std::pair<int, int> DisableWindowAwayPos()
{
    if(posX < -1.0) { posX = -1.0; }
    if(posX > 0.9)  { posX = 0.9;  }
    if(posY < -1.0) { posY = -1.0; }
    if(posY > 0.9)  { posY = 0.9;  }

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

void key_callback_end_state(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE || key == GLFW_KEY_ENTER || key == GLFW_KEY_SPACE)
        exit(0);
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
    
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    const GLubyte* shading_language = glGetString(GL_SHADING_LANGUAGE_VERSION); // version as a string

    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    printf("Supported GLSL version is %s.\n", shading_language);
    //
    
    glfwSetKeyCallback(window, key_callback);
    
    {
        
    float end_triangles[] = {
        0.9f, 0.9f, 0.0f, 0.0f,
        1.0f, 0.9f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        0.9f, 1.0f, 0.0f, 1.0f
    };
        
    float text_triangles[] = {
        -0.9f, -0.3f, 0.0f, 0.0f,
        -0.6f, -0.3f, 1.0f, 0.0f,
        -0.6f,  0.3f, 1.0f, 1.0f,
        -0.9f,  0.3f, 0.0f, 1.0f,
    };
    
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        
    VertexArray vao1, vao2, vao3;
    VertexBuffer vbo1(nullptr, 4 * 4 * sizeof(float), GL_DYNAMIC_DRAW);
    VertexBuffer vbo2(end_triangles, 4 * 4 * sizeof(float), GL_STATIC_DRAW);
    VertexBuffer vbo3(text_triangles, 4 * 4 * sizeof(float), GL_STATIC_DRAW);
    VertexBufferLayout layout1, layout2, layout3;
    IndexBuffer ibo(indices, 36);
        
    vao1.Bind();
    vbo1.Bind();
    layout1.PushFloat(2);
    layout1.PushFloat(2);
    vao1.AddBuffer(vbo1, layout1);
    ibo.Bind();

    vao2.Bind();
    vbo2.Bind();
    layout2.PushFloat(2);
    layout2.PushFloat(2);
    vao2.AddBuffer(vbo2, layout2);
    ibo.Bind();

    vao3.Bind();
    vbo3.Bind();
    layout3.PushFloat(2);
    layout3.PushFloat(2);
    vao3.AddBuffer(vbo3, layout3);
    ibo.Bind();
        
    glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);   // aspect ratio, prespective
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // camera position, orientation
        
    Shader shader("OpenGL_tutorial/basic.shader");
    Shader shader_maze("OpenGL_tutorial/maze.shader");
    shader_maze.Bind();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WIDTH), 0.0f, static_cast<float>(HEIGHT));
    glUniformMatrix4fv(glGetUniformLocation(shader_maze.GetShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    shader.Bind();
        
    Texture texture_player("OpenGL_tutorial/pepega.png");
    Texture texture_end("OpenGL_tutorial/end.png");
    Texture Y("OpenGL_tutorial/abeceda/y.png");
    Texture O("OpenGL_tutorial/abeceda/o.png");
    Texture U("OpenGL_tutorial/abeceda/u.png");
    Texture W("OpenGL_tutorial/abeceda/w.png");
    Texture I("OpenGL_tutorial/abeceda/i.png");
    Texture N("OpenGL_tutorial/abeceda/n.png");

    texture_player.Bind();
    shader.SetUniform1i("u_Texture", 0);
    
    vao1.Unbind();
    vao2.Unbind();
    vao3.Unbind();
    vbo1.Unbind();
    vbo2.Unbind();
    vbo3.Unbind();
    ibo.Unbind();
    shader.Unbind();
        
    Renderer renderer;
        
    renderer.InitializeFreetype();
        
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char *)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
        
    glm::vec3 translation(0.0f, 0, 0);
    
// MAZE
        float wall_lines[] = {
             0.0f, 0.0f, 0.0f, 0.0f,
             0.5f, 0.0f, 1.0f, 0.0f,
            
             0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.0f, 1.0f, 0.0f,
            
            -0.9f, 0.9f, 0.0f, 0.0f,
            -0.5f, 0.9f, 1.0f, 0.0f,
            
            -0.7f, 0.7f, 0.0f, 0.0f,
            -0.5f, 0.7f, 1.0f, 0.0f,
        };
        
        unsigned int indices_walls[] = {
            0, 1
        };
        
        int num_of_vertices = 8;
        VertexArray vao_walls;
        VertexBuffer vbo_walls(wall_lines, 4 * num_of_vertices * sizeof(float), GL_STATIC_DRAW);
        VertexBufferLayout walls_layout;
        IndexBuffer ibo_walls(indices, num_of_vertices);
        
        Texture texture_line("OpenGL_tutorial/line.jpg");
        
        vao_walls.Bind();
        vbo_walls.Bind();
        walls_layout.PushFloat(2);
        walls_layout.PushFloat(2);
        vao_walls.AddBuffer(vbo_walls, walls_layout);
        ibo_walls.Bind();
        
        
        
// END OF MAZE
        
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation); // object position, rotation, scale
        glm::mat4 mvp = proj * view * model;
        
        float player_triangles[] = {
            -1.0f + (posX + 1), -1.0f + (posY + 1), 0.0f, 0.0f,
            -0.9f + (posX + 1), -1.0f + (posY + 1), 1.0f, 0.0f,
            -0.9f + (posX + 1), -0.9f + (posY + 1), 1.0f, 1.0f,
            -1.0f + (posX + 1), -0.9f + (posY + 1), 0.0f, 1.0f
        };
        
        renderer.RenderText(shader_maze, "#", 0.0f, 25.0f, 0.4f, glm::vec3(0.8f, 0.2f, 0.2f));
        renderer.RenderText(shader_maze, "#", 33.0f, 2.0f, 0.4f, glm::vec3(0.8f, 0.2f, 0.2f));
        
        //RenderText(shader_maze, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        renderer.RenderText(shader_maze, "#", 0.0f, 466.0f, 0.4f, glm::vec3(0.8f, 0.2f, 0.2f)); // xpos, ypos, scale, glm::vec3 color
        renderer.RenderText(shader_maze, "#", 630.0f, 2.0f, 0.4f, glm::vec3(0.8f, 0.2f, 0.2f)); // xpos, ypos, scale, glm::vec3 color
        
        // usable xpos pixel range -> 0.0f - 630.0f
        // usable ypos pixel range -> 2.0f - 466.0f
        
        // player width in pixels = 33.0f
        // player height in pixels = 25.0f
        
        shader.Bind();
        
        vbo1.Bind();
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * 4 * sizeof(float), player_triangles));
        
        shader.SetUniformMat4f("u_MVP", mvp);
        
        texture_player.Bind();
        renderer.Draw(vao1, ibo, shader);
        
        texture_end.Bind();
        renderer.Draw(vao2, ibo, shader);
        
        texture_line.Bind();
        renderer.DrawWalls(vao_walls, ibo_walls, shader);
        
        // Win
        if(posX >= 0.81f && posY >= 0.81f)
        {
            renderer.ClearWholeScreen();
            glfwSetKeyCallback(window, key_callback_end_state);
            
        // "YOU WIN" text
            
            model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0, 0));
            glm::mat4 mvp_text = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp_text);
            Y.Bind();
            renderer.Draw(vao3, ibo, shader);
            
            model = glm::translate(glm::mat4(1.0f), glm::vec3(0.25, 0, 0));
            mvp_text = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp_text);
            O.Bind();
            renderer.Draw(vao3, ibo, shader);

            model = glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0, 0));
            mvp_text = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp_text);
            U.Bind();
            renderer.Draw(vao3, ibo, shader);

            model = glm::translate(glm::mat4(1.0f), glm::vec3(1, 0.1, 0));
            mvp_text = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp_text);
            W.Bind();
            renderer.Draw(vao3, ibo, shader);

            model = glm::translate(glm::mat4(1.0f), glm::vec3(1.25, 0.1, 0));
            mvp_text = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp_text);
            I.Bind();
            renderer.Draw(vao3, ibo, shader);

            model = glm::translate(glm::mat4(1.0f), glm::vec3(1.5, 0.1, 0));
            mvp_text = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp_text);
            N.Bind();
            renderer.Draw(vao3, ibo, shader);
            
//            renderer.ClearWholeScreen();
//            RenderText(shader_maze, "YOU WIN", 250.0f, 200.0f, 0.7f, glm::vec3(0.8f, 0.2f, 0.2f));
        }
        else
        {
            glfwSetKeyCallback(window, key_callback);
        }
        
        {
            ImGui::SliderFloat3("Translation", &translation.x, -1.0f, 1.0f);
            ImGui::SliderFloat2("PosX", &posX, -1.0f, 0.9f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    
    glfwTerminate();
    return 0;
}
