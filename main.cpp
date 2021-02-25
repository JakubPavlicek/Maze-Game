#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Libraries.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "Maze.h"

static float posX = 0.0f, posY = 0.0f;

static int WIDTH = 640, HEIGHT = 480;

std::vector<Positions> avaibleN, avaibleE, avaibleS, avaibleW;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(posX < 0.0f)    { posX = 0.0f;   }
    if(posX > 620.0f)  { posX = 620.0f; }
    if(posY < 0.0f)    { posY = 0.0f;   }
    if(posY > 460.0f)  { posY = 460.0f; }

    switch(key)
    {
        default:
            if(action == GLFW_PRESS || action == GLFW_REPEAT)
                std::cout << "spatne tlacitko" << std::endl;
            break;
        
        case GLFW_KEY_ESCAPE: exit(0);
            
        case GLFW_KEY_UP:
            switch(action)
            {
                case GLFW_PRESS:
                    for(auto vec : avaibleN)
                    {
                        if(vec.x * 23 == posX && vec.y * 23 == posY)
                        {
                            posY += 23.0f;
                            break;
                        }
                    }
                    break;
                case GLFW_REPEAT:
                    for(auto vec : avaibleN)
                    {
                        if(vec.x * 23 == posX && vec.y * 23 == posY)
                        {
                            posY += 23.0f;
                            break;
                        }
                    }
                    break;
            }
            break;
            
        case GLFW_KEY_DOWN:
            switch(action)
            {
                case GLFW_PRESS:
                    for(auto vec : avaibleS)
                    {
                        if(vec.x * 23 == posX && vec.y * 23 == posY)
                        {
                            posY -= 23.0f;
                            break;
                        }
                    }
                    break;
                case GLFW_REPEAT:
                    for(auto vec : avaibleS)
                    {
                        if(vec.x * 23 == posX && vec.y * 23 == posY)
                        {
                            posY -= 23.0f;
                            break;
                        }
                    }
                    break;
            }
            break;
            
        case GLFW_KEY_RIGHT:
            switch(action)
            {
                case GLFW_PRESS:
                    for(auto vec : avaibleE)
                    {
                        if(vec.x * 23 == posX && vec.y * 23 == posY)
                        {
                            posX += 23.0f;
                            break;
                        }
                    }
                    break;
                case GLFW_REPEAT:
                    for(auto vec : avaibleE)
                    {
                        if(vec.x * 23 == posX && vec.y * 23 == posY)
                        {
                            posX += 23.0f;
                            break;
                        }
                    }
                    break;
            }
            break;
            
        case GLFW_KEY_LEFT:
            switch(action)
            {
                case GLFW_PRESS:
                    for(auto vec : avaibleW)
                    {
                        if(vec.x * 23 == posX && vec.y * 23 == posY)
                        {
                            posX -= 23.0f;
                            break;
                        }
                    }
                    break;
                case GLFW_REPEAT:
                    for(auto vec : avaibleW)
                    {
                        if(vec.x * 23 == posX && vec.y * 23 == posY)
                        {
                            posX -= 23.0f;
                            break;
                        }
                    }
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
 
    {
        
    float end_vertices[] = {
        621.0f, 460.0f, 0.0f, 0.0f,
        641.0f, 460.0f, 1.0f, 0.0f,
        641.0f, 480.0f, 1.0f, 1.0f,
        621.0f, 480.0f, 0.0f, 1.0f
    };
        
    float cell_vertices[] = {
        0.0f,  0.0f,  0.0f, 0.0f,
        20.0f, 0.0f,  1.0f, 0.0f,
        20.0f, 20.0f, 1.0f, 1.0f,
        0.0f,  20.0f, 0.0f, 1.0f
    };
        
    float path_verticesX[] = {
        0.0f,  20.0f,  0.0f, 0.0f,
        20.0f, 20.0f,  1.0f, 0.0f,
        20.0f, 23.0f, 1.0f, 1.0f,
        0.0f,  23.0f, 0.0f, 1.0f
    };
    
    float path_verticesY[] = {
        20.0f, 0.0f,  0.0f, 0.0f,
        23.0f, 0.0f,  1.0f, 0.0f,
        23.0f, 20.0f, 1.0f, 1.0f,
        20.0f, 20.0f, 0.0f, 1.0f
    };
        
    float text_vertices[] = {
        20.0f,  160.0f, 0.0f, 0.0f,
        100.0f, 160.0f, 1.0f, 0.0f,
        100.0f, 320.0f, 1.0f, 1.0f,
        20.0f,  320.0f, 0.0f, 1.0f,
    };
    
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        
    VertexArray vao1, vao2, vao3, vao4, vao5, vao6;
    VertexBuffer vbo1(nullptr, 4 * 4 * sizeof(float), GL_DYNAMIC_DRAW);
    VertexBuffer vbo2(end_vertices, 4 * 4 * sizeof(float), GL_STATIC_DRAW);
    VertexBuffer vbo3(cell_vertices, 4 * 4 * sizeof(float), GL_STATIC_DRAW);
    VertexBuffer vbo4(text_vertices, 4 * 4 * sizeof(float), GL_STATIC_DRAW);
    VertexBuffer vbo5(path_verticesX, 4 * 4 * sizeof(float), GL_STATIC_DRAW);
    VertexBuffer vbo6(path_verticesY, 4 * 4 * sizeof(float), GL_STATIC_DRAW);
    VertexBufferLayout layout1, layout2, layout3, layout4, layout5, layout6;
    IndexBuffer ibo(indices, 6);
        
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
        
    vao4.Bind();
    vbo4.Bind();
    layout4.PushFloat(2);
    layout4.PushFloat(2);
    vao4.AddBuffer(vbo4, layout4);
    ibo.Bind();
    
    vao5.Bind();
    vbo5.Bind();
    layout5.PushFloat(2);
    layout5.PushFloat(2);
    vao5.AddBuffer(vbo5, layout5);
    ibo.Bind();
    
    vao6.Bind();
    vbo6.Bind();
    layout6.PushFloat(2);
    layout6.PushFloat(2);
    vao6.AddBuffer(vbo6, layout6);
    ibo.Bind();
        
    glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(WIDTH), 0.0f, static_cast<float>(HEIGHT));   // aspect ratio, prespective
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
        
    Texture texture_white("OpenGL_tutorial/white.jpg");
    Texture texture_blue("OpenGL_tutorial/blue.png");
    Texture texture_red("OpenGL_tutorial/red.png");

    texture_player.Bind();
    shader.SetUniform1i("u_Texture", 0);

    Renderer renderer;
        
    renderer.InitializeFreetype();
        
    renderer.InitializeImGui();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
        
    glfwSetKeyCallback(window, key_callback);
        
    glm::vec3 translation(0.0f, 0, 0);
    
// MAZE
    Maze maze(644, 483);
        
    std::srand((unsigned int)(std::time(0)));
        
    maze.MakeMaze();
        
    avaibleN = maze.vector_of_N_pos;
    avaibleE = maze.vector_of_E_pos;
    avaibleS = maze.vector_of_S_pos;
    avaibleW = maze.vector_of_W_pos;
        
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
            0.0f  + (posX), 0.0f  + (posY), 0.0f, 0.0f,
            20.0f + (posX), 0.0f  + (posY), 1.0f, 0.0f,
            20.0f + (posX), 20.0f + (posY), 1.0f, 1.0f,
            0.0f  + (posX), 20.0f + (posY), 0.0f, 1.0f
        };
        
        shader.Bind();
        
        vbo1.Bind();
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * 4 * sizeof(float), player_triangles));
        
        shader.SetUniformMat4f("u_MVP", mvp);
// MAZE
        maze.DrawMaze(texture_white, texture_blue, vao3, vao5, vao6, ibo);
        
// PLAYER AND END
        shader.SetUniformMat4f("u_MVP", mvp);
        model = glm::translate(glm::mat4(1.0f), translation); // object position, rotation, scale
        mvp = proj * view * model;
        
        texture_end.Bind();
        renderer.Draw(vao2, ibo, shader);
        
        texture_player.Bind();
        renderer.Draw(vao1, ibo, shader);

        // Win
        if(posX >= 617.0f && posY >= 457.0f)
        {
            renderer.ClearWholeScreen();
            glfwSetKeyCallback(window, key_callback_end_state);
            
        // "YOU WIN" text
            renderer.SetLetter(50.0f, 0.0f, proj, view, shader);
            Y.Bind();
            renderer.Draw(vao4, ibo, shader);

            renderer.SetLetter(130.0f, 0.0f, proj, view, shader);
            O.Bind();
            renderer.Draw(vao4, ibo, shader);

            renderer.SetLetter(210.0f, 0.0f, proj, view, shader);
            U.Bind();
            renderer.Draw(vao4, ibo, shader);
            
            renderer.SetLetter(310.0f, 25.0f, proj, view, shader);
            W.Bind();
            renderer.Draw(vao4, ibo, shader);

            renderer.SetLetter(390.0f, 25.0f, proj, view, shader);
            I.Bind();
            renderer.Draw(vao4, ibo, shader);

            renderer.SetLetter(470.0f, 25.0f, proj, view, shader);
            N.Bind();
            renderer.Draw(vao4, ibo, shader);
        }
        else
        {
            glfwSetKeyCallback(window, key_callback);
        }
        
        {
            ImGui::SliderFloat3("Translation", &translation.x, 0, 640.0f);
            ImGui::SliderFloat2("PosX", &posX, 0.0f, 640.0f);
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
