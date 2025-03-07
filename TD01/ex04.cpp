#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glbasimac/glbi_engine.hpp"
#include <iostream>

using namespace glbasimac;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;
static float aspectRatio = 1.0f;

float red = 0.2f;
float green = 0.f;
float blue = 0.f;

/* Espace virtuel */
static const float GL_VIEW_SIZE = 1.;

/* OpenGL Engine */
GLBI_Engine myEngine;

/* Error handling function */
void onError(int error, const char *description)
{
    std::cout << "GLFW Error (" << error << ") : " << description << std::endl;
}

void onWindowResized(GLFWwindow * /*window*/, int width, int height) {
    aspectRatio = width / (float)height;
    glViewport(0, 0, width, height);

    if(aspectRatio > 1.0) {
        myEngine.set2DProjection(-GL_VIEW_SIZE * aspectRatio / 2., GL_VIEW_SIZE * aspectRatio / 2., -GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.);
    } else {
        myEngine.set2DProjection(-GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2., -GL_VIEW_SIZE / (2. * aspectRatio), GL_VIEW_SIZE / (2. * aspectRatio));
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // if (key == GLFW_KEY_A && action == GLFW_PRESS) { // Ici on utiliser GLFW_KEY_A car glfw se base sur un clavier qwerty (donc Q pour un azerty)
    //     glfwSetWindowShouldClose(window, GLFW_TRUE);
    // }

    const char* keyName = glfwGetKeyName(key, scancode);
    // std::cout << "Key: " << keyName << std::endl;
    if(keyName[0] == 'q' && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        // std::cout << "Mouse position: " << xpos << ", " << ypos << std::endl;

        red = static_cast<float>(static_cast<int>(xpos) % 256) / 255.0f;
        green = static_cast<float>(static_cast<int>(ypos) % 256) / 255.0f;
        blue = 0.f;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    // std::cout << "Mouse position: " << xpos << ", " << ypos << std::endl;
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    red = static_cast<float>(static_cast<int>(xpos) % width) / static_cast<float>(width);
    green = 0.f;
    blue = static_cast<float>(static_cast<int>(ypos) % height) / static_cast<float>(height);
}

int main()
{
    // Initialize the library
    if (!glfwInit())
    {
        return -1;
    }

    /* Callback to a function if an error is rised by GLFW */
    glfwSetErrorCallback(onError);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(800, 800, "TD 01 Ex 04", nullptr, nullptr);
    // GLFWwindow *window = glfwCreateWindow(800, 800, "TD 01 Ex 04", glfwGetPrimaryMonitor(), nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwSetWindowSizeCallback(window, onWindowResized);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // Make the window's context current
    glfwMakeContextCurrent(window);
    
    // Intialize glad (loads the OpenGL functions)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }
    
    // Initialize Rendering Engine
    myEngine.initGL();

    onWindowResized(window, 800, 800);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Get time (in second) at loop beginning */
        double startTime = glfwGetTime();

        /* Render here */
        glClearColor(red, green, blue, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render here

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        /* Elapsed time computation from loop begining */
        double elapsedTime = glfwGetTime() - startTime;
        /* If to few time is spend vs our wanted FPS, we wait */
        while (elapsedTime < FRAMERATE_IN_SECONDS)
        {
            glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS - elapsedTime);
            elapsedTime = glfwGetTime() - startTime;
        }
    }

    glfwTerminate();
    return 0;
}
