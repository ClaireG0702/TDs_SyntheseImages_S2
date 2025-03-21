#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include <iostream>

using namespace glbasimac;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;
static float aspectRatio = 1.0f;

/* Espace virtuel */
static const float GL_VIEW_SIZE = 4.0;

/* OpenGL Engine */
GLBI_Engine myEngine;
GLBI_Set_Of_Points thePoints;
GLBI_Convex_2D_Shape carre;
GLBI_Convex_2D_Shape triangle;
GLBI_Convex_2D_Shape cercle;

int objectNumber = 0;


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

void initScene() {
    std::vector<float> carreCoordinates = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f
    };
    std::vector<float> triangleCoordinates = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.0f, 0.5f
    };
    std::vector<float> cercleCoordinates;
    int numSegments = 50; // Number of segments to approximate the circle
    float angleStep = 2.0f * M_PI / numSegments;
    for (int i = 0; i < numSegments; ++i) {
        float angle = i * angleStep;
        float x = cos(angle);
        float y = sin(angle);
        cercleCoordinates.push_back(x);
        cercleCoordinates.push_back(y);
    }

    carre.initShape(carreCoordinates);
    triangle.initShape(triangleCoordinates);
    cercle.initShape(cercleCoordinates);
}

void renderScene() {
    switch(objectNumber) {
    case 0:
        myEngine.setFlatColor(1.0f, 0.0f, 0.0f);
        carre.drawShape();
        break;
    case 1:
        myEngine.setFlatColor(1.0f, 1.0f, 0.0f);
        triangle.drawShape();
        break;
    case 2:
        myEngine.setFlatColor(0.0f, 1.0f, 0.0f);
        cercle.drawShape();
        break;
    default:
        break;
    }
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        // std::cout << "Mouse position: " << xpos << ", " << ypos << std::endl;
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Call onWindowResized to update aspectRatio
        onWindowResized(window, width, height);

        float x = (float)xpos / (width / 2.f) - 1.0f;
        float y = 1.0f - (float)ypos / (height / 2.f);

        if (aspectRatio > 1.0f) {
            x *= aspectRatio;
        } else {
            y /= aspectRatio;
        }

        float pointCoord[2] = { x, y };
        float pointColor[3] = { 1.0f, 1.0f, 1.0f }; // White color

        thePoints.addAPoint(pointCoord, pointColor);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    const char* keyName = glfwGetKeyName(key, scancode);

    if (action == GLFW_PRESS && keyName[0] == 'b') {
        objectNumber = (objectNumber+1)%3;
    }
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
    GLFWwindow *window = glfwCreateWindow(800, 800, "TD 02 Ex 04", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwSetWindowSizeCallback(window, onWindowResized);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    
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

    // Initaialize the set of points
    initScene();
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Get time (in second) at loop beginning */
        double startTime = glfwGetTime();

        /* Render here */
        glClearColor(0.2f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render here
        renderScene();

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
