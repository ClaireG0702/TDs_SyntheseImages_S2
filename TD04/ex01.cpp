#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include <iostream>
#include "draw_scene.hpp"

using namespace glbasimac;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;
static float aspectRatio = 1.0f;

/* Espace virtuel */
static const float GL_VIEW_SIZE = 50.0;


/* Error handling function */
void onError(int error, const char *description)
{
    std::cout << "GLFW Error (" << error << ") : " << description << std::endl;
}

void onWindowResized(GLFWwindow * /*window*/, int width, int height) {
    aspectRatio = width / (float)height;
    glViewport(0, 0, width, height);

    myEngine.set3DProjection(90.0,aspectRatio,Z_NEAR,Z_FAR);
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // double xpos, ypos;
        // glfwGetCursorPos(window, &xpos, &ypos);
        // // std::cout << "Mouse position: " << xpos << ", " << ypos << std::endl;
        // int width, height;
        // glfwGetWindowSize(window, &width, &height);

        // // Call onWindowResized to update aspectRatio
        // onWindowResized(window, width, height);

        // float x = (float)xpos / (width / 2.f) - 1.0f;
        // float y = 1.0f - (float)ypos / (height / 2.f);

        // if (aspectRatio > 1.0f) {
        //     x *= aspectRatio;
        // } else {
        //     y /= aspectRatio;
        // }
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // const char* keyName = glfwGetKeyName(key, scancode);

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_F:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            case GLFW_KEY_P:
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
            case GLFW_KEY_UP:
                angle_phy += 1.0;
                break;
            case GLFW_KEY_DOWN:
                angle_phy -= 1.0;
                break;
            case GLFW_KEY_LEFT:
                angle_theta += 1.0;
                break;
            case GLFW_KEY_RIGHT:
                angle_theta -= 1.0;
                break;
            case GLFW_KEY_1:
                dist_zoom *= 0.9;
                break;
            case GLFW_KEY_2:
                dist_zoom *= 1.1;
                break;
            default:
                break;
        }
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
    GLFWwindow *window = glfwCreateWindow(800, 800, "TD 04", nullptr, nullptr);
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

    myEngine.mode2D = false; // Set engine to 3D mode
    
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        /* Fix camera position */
        myEngine.mvMatrixStack.loadIdentity();
        Vector3D pos_camera =
            Vector3D(dist_zoom*cos(deg2rad(angle_theta))*cos(deg2rad(angle_phy)),
            dist_zoom*sin(deg2rad(angle_theta))*cos(deg2rad(angle_phy)),
            dist_zoom*sin(deg2rad(angle_phy)));
        Vector3D viewed_point = Vector3D(0.0,0.0,0.0);
        Vector3D up_vector = Vector3D(0.0,0.0,1.0);
        Matrix4D viewMatrix = Matrix4D::lookAt(pos_camera,viewed_point,up_vector);
        myEngine.setViewMatrix(viewMatrix);
        myEngine.updateMvMatrix();

        drawScene();

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
