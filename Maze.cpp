#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Maze.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch(key)
    {
    default:
        if(action == GLFW_PRESS)
            std::cout << "spatne tlacitko" << std::endl;
        break;

    case GLFW_KEY_UP:
        if(action == GLFW_PRESS)
            std::cout << "Nahoru" << std::endl;
        if(action == GLFW_REPEAT)
            std::cout << "Nahoru - opak" << std::endl;
        break;

    case GLFW_KEY_DOWN:
        if(action == GLFW_PRESS)
            std::cout << "Dolu" << std::endl;
        if(action == GLFW_REPEAT)
            std::cout << "Dolu - opak" << std::endl;
        break;

    case GLFW_KEY_LEFT:
        if(action == GLFW_PRESS)
            std::cout << "Doleva" << std::endl;
        if(action == GLFW_REPEAT)
            std::cout << "Doleva - opak" << std::endl;
        break;

    case GLFW_KEY_RIGHT:
        if(action == GLFW_PRESS)
            std::cout << "Doprava" << std::endl;
        if(action == GLFW_REPEAT)
            std::cout << "Doprava - opak" << std::endl;
        break;
    }
}

int Maze::createMaze(float height, float width)
{
    GLFWwindow* window;
    /* Initialize the library */
    if(!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow((int)height, (int)width, "Maze Game", NULL, NULL);
    if(!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    float pointVertex2[] = {
        1280 * 0.75, 360
    };

    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    /* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glEnable(GL_POINT_SMOOTH);
        glBegin(GL_ARRAY_BUFFER);
        glVertexPointer(2, GL_FLOAT, 0, pointVertex2);
        glPointSize(50);
        glDrawArrays(GL_POINTS, 0, 1);
        glEnd();
        glDisable(GL_POINT_SMOOTH);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void Maze::resetMaze()
{

}

void Maze::movePlayer()
{
    
}

int main(void)
{
    Maze game;
    game.createMaze(1280, 720);
    
}
