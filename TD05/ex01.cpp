#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_texture.hpp"
#include "draw_scene.hpp"
#include "tools/shaders.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "tools/stb_image.h"
#include <iostream>

using namespace glbasimac;
using namespace STP3D;

/* Window properties */
static const unsigned int WINDOW_WIDTH = 1200;
static const unsigned int WINDOW_HEIGHT = 800;
static const char WINDOW_TITLE[] = "TD05 Ex01";
static float aspectRatio = 1.0f;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* 3D Engine global variables */
StandardMesh* rectangle;
IndexedMesh* sphere2;
StandardMesh* a_frame;

GLBI_Texture myTexture;

/* Error handling function */
void onError(int error, const char* description) {
	std::cout << "GLFW Error ("<<error<<") : " << description << std::endl;
}

void onWindowResized(GLFWwindow* /*window*/, int width, int height)
{
	aspectRatio = width / (float) height;

	glViewport(0, 0, width, height);
	std::cerr<<"Setting 3D projection"<<std::endl;
	myEngine.set3DProjection(60.0,aspectRatio,Z_NEAR,Z_FAR);
}

void onKey(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
	int is_pressed = (action == GLFW_PRESS); 
	switch(key) {
		case GLFW_KEY_A :
		case GLFW_KEY_ESCAPE :
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_L:
			if (is_pressed) glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			break;
		case GLFW_KEY_P:
			if (is_pressed) glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			break;
        case GLFW_KEY_UP :
            angle_phy += 1.0;
            break;
        case GLFW_KEY_DOWN :
            angle_phy -= 1.0;
            break;
        case GLFW_KEY_I :
            dist_zoom *= 0.95;
            break;
        case GLFW_KEY_K :
            dist_zoom *= 1.05;
            break;
        case GLFW_KEY_LEFT :
            angle_theta += 1.0;
            break;
        case GLFW_KEY_RIGHT :
            angle_theta -= 1.0;
            break;
		default: std::cerr<<"Touche non gérée "<<key<<std::endl;
	}

}

void onMouseButton(GLFWwindow* window, int button, int action, int /*mods*/)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout<<"Pressed in "<<xpos<<" "<<ypos<<std::endl;

	}
}
void initBasicScene() {
	// rectangle = basicRect(10.0,10.0);
	// rectangle->createVAO();
	sphere2 = basicSphere(10.0, 20, 20);
	sphere2->createVAO();

	a_frame = createRepere(10.0);
	a_frame->createVAO();

	int width, height, channelsNbr;
	unsigned char* ptr_img = stbi_load("../assets/textures/triforce.jpg", &width, &height, &channelsNbr, 0);
	if(ptr_img == nullptr) {
		std::cerr << "Unable to load image" << std::endl;
		exit(1);
	}

	myTexture.createTexture();
	myTexture.attachTexture();
	myTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	myTexture.loadImage(width, height, channelsNbr, ptr_img);

	myTexture.detachTexture();
	stbi_image_free(ptr_img);
}

void renderBasicScene() {
	a_frame->draw();
	myEngine.setFlatColor(1.0,0.0,0.0);

	myEngine.activateTexturing(true);
	myTexture.attachTexture();

	// rectangle->draw();
	sphere2->draw();

	myTexture.detachTexture();
}

int main(int /*argc*/, char** /*argv*/)
{
	/* GLFW initialisation */
	GLFWwindow* window;
	if (!glfwInit()) return -1;

	/* Callback to a function if an error is rised by GLFW */
	glfwSetErrorCallback(onError);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!window)
	{
		// If no context created : exit !
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	std::cout<<"Loading GL extension"<<std::endl;
	// Intialize glad (loads the OpenGL functions)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return -1;
	}

	glfwSetWindowSizeCallback(window,onWindowResized);
	glfwSetKeyCallback(window, onKey);
	glfwSetMouseButtonCallback(window, onMouseButton);

	std::cout<<"Engine init"<<std::endl;
	myEngine.mode2D = false; // Set engine to 3D mode
	myEngine.initGL();
	onWindowResized(window,WINDOW_WIDTH,WINDOW_HEIGHT);
	CHECK_GL;

	initBasicScene();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Render begins here */
		glClearColor(0.f,0.0f,0.2f,0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		
		/* Fix camera position */
		myEngine.mvMatrixStack.loadIdentity();

		Vector3D pos_camera = Vector3D(dist_zoom*cos(deg2rad(angle_theta))*cos(deg2rad(angle_phy)),
										dist_zoom*sin(deg2rad(angle_theta))*cos(deg2rad(angle_phy)),
										dist_zoom*sin(deg2rad(angle_phy)));
		Vector3D viewed_point = Vector3D(0.0,0.0,0.0);
		Vector3D up_vector = Vector3D(0.0,0.0,1.0); // DO NOT TOUCH IT
		Matrix4D viewMatrix = Matrix4D::lookAt(pos_camera,viewed_point,up_vector);
		myEngine.setViewMatrix(viewMatrix);
		myEngine.updateMvMatrix();

		renderBasicScene();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */
		double elapsedTime = glfwGetTime() - startTime;
		/* If to few time is spend vs our wanted FPS, we wait */
		if(elapsedTime < FRAMERATE_IN_SECONDS)
		{
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS-elapsedTime);
		}
	}

	glfwTerminate();

	return 0;
}
