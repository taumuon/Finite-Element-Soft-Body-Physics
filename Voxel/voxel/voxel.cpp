#include "pch.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <ios>
#include <fstream>
#include <string>
#include <iostream>

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
using namespace glm;

#include "Windows.h"

#include "Game.h"
#include "FEA.h"

#if _DEBUG
static void CALLBACK debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *data)
{
	std::cout << "debug call: " << msg << std::endl;
}
#endif

int main()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	//glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	// To get an ES context, but will need to change shaders to be compatible.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

#if _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glEnable(GL_DEBUG_OUTPUT);
#endif

	// Open a window and create its OpenGL context
	GLFWwindow* window; // (In the accompanying source code, this variable is global)
	auto width = 1024;
	auto height = 768;
	window = glfwCreateWindow(1024, 768, "Soft body physics with FEA", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}

	//if (ogl_ext_KHR_debug) {
	//	std::cout << "KHR_debug supported" << std::endl;
	//}

#if _DEBUG
	GLint v;
	glGetIntegerv(GL_CONTEXT_FLAGS, &v);
	if (v & GL_CONTEXT_FLAG_DEBUG_BIT) {
		std::cout << "OpenGL debug context present" << std::endl;
	}
#endif

	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

#if _DEBUG
	glDebugMessageCallback(debugCallback, nullptr);
#endif

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	Game game;
	game.UpdateWindowSize(width, height);

	// TODO: do this in Android code too - worth pulling out common initialization into renderer?
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game.Update();
		game.Draw();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

    return 0;
}

