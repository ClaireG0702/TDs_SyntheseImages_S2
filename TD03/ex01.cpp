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
static const float GL_VIEW_SIZE = 100.0;

/* OpenGL Engine */
GLBI_Engine myEngine;
GLBI_Set_Of_Points thePoints;
GLBI_Convex_2D_Shape carre;
GLBI_Convex_2D_Shape triangle;
GLBI_Convex_2D_Shape cercle;
GLBI_Convex_2D_Shape trapeze;

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
    std::vector<float> trapezeCoordinates = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.25f, 0.5f,
        -0.25f, 0.5f
    };

    carre.initShape(carreCoordinates);
    triangle.initShape(triangleCoordinates);
    cercle.initShape(cercleCoordinates);
    trapeze.initShape(trapezeCoordinates);
}

void drawFirstArm() {
    myEngine.setFlatColor(1.f, 1.f, 1.f);

    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({ -20.f, 0.f, 0.f });

        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addHomothety({ 20.f, 20.f, 1.f });
            myEngine.updateMvMatrix();
            cercle.changeNature(GL_TRIANGLE_FAN);
            cercle.drawShape();
        myEngine.mvMatrixStack.popMatrix();

        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addRotation(-M_PI/2, { 0.f, 0.f, 1.f });
            myEngine.mvMatrixStack.addTranslation({ 0.f, 30.f, 0.f });
            myEngine.mvMatrixStack.addHomothety({ 40.f, 60.f, 0.f });
            myEngine.updateMvMatrix();  
            trapeze.changeNature(GL_TRIANGLE_FAN);
            trapeze.drawShape();
        myEngine.mvMatrixStack.popMatrix();

        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation({ 60.f, 0.f, 0.f });
            myEngine.mvMatrixStack.addHomothety({ 10.f, 10.f, 1.f });
            myEngine.updateMvMatrix();
            cercle.changeNature(GL_TRIANGLE_FAN);
            cercle.drawShape();
        myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.popMatrix();
}

void drawRoundedSquare() {
    myEngine.setFlatColor(1.f, 1.f, 1.f);

    // Dessiner le carré
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addHomothety({ 1.f, 0.8f, 1.f });
        myEngine.updateMvMatrix();
            carre.changeNature(GL_TRIANGLE_FAN);
        carre.drawShape();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addHomothety({ 0.8f, 1.f, 1.f });
        myEngine.updateMvMatrix();
            carre.changeNature(GL_TRIANGLE_FAN);
        carre.drawShape();
    myEngine.mvMatrixStack.popMatrix();
    
    float cornerRadius = 0.1f;

    // Dessiner les cercles aux coins
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({ 0.5f - cornerRadius, 0.5f - cornerRadius, 0.f });
        myEngine.mvMatrixStack.addHomothety({ cornerRadius, cornerRadius, 1.f });
        myEngine.updateMvMatrix();
        cercle.changeNature(GL_TRIANGLE_FAN);
        cercle.drawShape();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({ -0.5f + cornerRadius, 0.5f - cornerRadius, 0.f });
        myEngine.mvMatrixStack.addHomothety({ cornerRadius, cornerRadius, 1.f });
        myEngine.updateMvMatrix();
        cercle.changeNature(GL_TRIANGLE_FAN);
        cercle.drawShape();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({ 0.5f - cornerRadius, -0.5f + cornerRadius, 0.f });
        myEngine.mvMatrixStack.addHomothety({ cornerRadius, cornerRadius, 1.f });
        myEngine.updateMvMatrix();
        cercle.changeNature(GL_TRIANGLE_FAN);
        cercle.drawShape();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({ -0.5f + cornerRadius, -0.5f + cornerRadius, 0.f });
        myEngine.mvMatrixStack.addHomothety({ cornerRadius, cornerRadius, 1.f });
        myEngine.updateMvMatrix();
        cercle.changeNature(GL_TRIANGLE_FAN);
        cercle.drawShape();
    myEngine.mvMatrixStack.popMatrix();
}

void drawSecondArm() {
    myEngine.setFlatColor(1.f, 1.f, 1.f);

    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addHomothety({ 10.f, 10.f, 1.f });
        myEngine.updateMvMatrix();
        drawRoundedSquare();
    myEngine.mvMatrixStack.popMatrix();

    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({ 46.f-6.f, 0.f, 0.f });
        myEngine.mvMatrixStack.addHomothety({ 10.f, 10.f, 1.f });
        myEngine.updateMvMatrix();
        drawRoundedSquare();
    myEngine.mvMatrixStack.popMatrix();

    myEngine.setFlatColor(0.5f, 0.5f, 0.5f);
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({ 23.f-3.f, 0.f, 0.f });
        myEngine.mvMatrixStack.addHomothety({ 46.f, 6.f, 1.f });
        myEngine.updateMvMatrix();
        carre.changeNature(GL_TRIANGLE_FAN);
        carre.drawShape();
    myEngine.mvMatrixStack.popMatrix();
}

void drawThirdArm() {
    myEngine.setFlatColor(0.5f, 0.5f, 0.5f);

    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addHomothety({ 6.f, 6.f, 1.f });
        myEngine.updateMvMatrix();
        carre.changeNature(GL_TRIANGLE_FAN);
        carre.drawShape();
    myEngine.mvMatrixStack.popMatrix();    

    myEngine.setFlatColor(0.f, 0.f, 0.f);
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({ 20.f-2.f, 0.f, 0.f });
        myEngine.mvMatrixStack.addHomothety({ 40.f, 4.f, 1.f });
        myEngine.updateMvMatrix();
        carre.changeNature(GL_TRIANGLE_FAN);
        carre.drawShape();
    myEngine.mvMatrixStack.popMatrix();

    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({ 40.f-2.f, 0.f, 0.f });
        myEngine.mvMatrixStack.addHomothety({ 4.f, 4.f, 1.f });
        myEngine.updateMvMatrix();
        cercle.changeNature(GL_TRIANGLE_FAN);
        cercle.drawShape();
    myEngine.mvMatrixStack.popMatrix();
}

void renderScene() {
    drawThirdArm();
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

    if (action == GLFW_PRESS && keyName[0] == 'f') {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (action == GLFW_PRESS && keyName[0] == 'p') {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
    GLFWwindow *window = glfwCreateWindow(800, 800, "TD 03 Ex 01", nullptr, nullptr);
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
