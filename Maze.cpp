#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Maze.h"

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


    /* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window))
    {
        int DOWN = glfwGetKey(window, GLFW_KEY_DOWN);
        int UP   = glfwGetKey(window, GLFW_KEY_UP);
        int LEFT = glfwGetKey(window, GLFW_KEY_LEFT);
        int RIHT = glfwGetKey(window, GLFW_KEY_RIGHT);

        if(DOWN == GLFW_PRESS)
        {
            std::cout << "wtf" << std::endl;  
        }

        if(UP == GLFW_PRESS)
        {
            std::cout << "omg" << std::endl;
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

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
    Maze o_maze;
    o_maze.createMaze(1280, 720);
    
    //unsigned int buffer;
    //glGenBuffers(1, &buffer);
    //glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //glBufferData();

    
}
