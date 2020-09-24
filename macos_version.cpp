#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdio>

float posX = 0, posY = 0;

int WIDTH = 640, HEIGHT = 480;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
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

int main(void)
{
    GLFWwindow* window;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Maze Game", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
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
//
//        glLineWidth(5);
//        glBegin(GL_LINES);
//        glVertex2f(0, -0.5);
//        glVertex2f(1, -0.5);
//        glEnd();
        
        glLineWidth(5);
        glBegin(GL_LINES);
        glVertex2f(-1, -0.3);
        glVertex2f(0, -0.3);
        glEnd();
        
        // Lose
        if(posX >= -0.1f && posX <= 1.0f && posY >= 0.65f && posY <= 0.7f)
            std::cout << "PROHRAL SI" << std::endl;
        
        // Win
        if(posX >= 1.8 && posY >= 1.8)
            exit(0);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    
    return 0;
}
