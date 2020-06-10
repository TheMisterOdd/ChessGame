#pragma once

#include <stdio.h>
#include <malloc.h>
#include "Types.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Input.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

static float deltaTime = 0.0f, lastTime = 0.0f;
char* CPU_Info;

static void __GLFWwindowResizeCallback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
}

static void __GLFWwindowErrCallback(int err, const char* description)
{
	printf("ERROR: %s\n", description);
}

typedef struct
{
	int width, height, fullscreen;

	GLFWwindow* window;
	GLFWvidmode* vidMode;
	Input input;

} Window;

/*! @brief Sets a window cursor
 *
 *  @param[in] window is the window that is going to be used for setting the icon.
 *  @param[in] path the source where the image is found.
 *
 *  @errors Window cursor could not be setted
 */
GLFWcursor* WindowSetCursor(Window* window, const char* path)
{
	GLFWimage* image = (GLFWimage*)malloc(sizeof(GLFWimage));
	image[0].pixels = stbi_load(path, &image[0].width, &image[0].height, NULL, STBI_rgb_alpha);
	GLFWcursor* cursor = glfwCreateCursor(image, 0, 0);

	glfwSetCursor(window->window, cursor);
	free(image);

	return cursor;
}

/*! @brief Sets a window icon
 *
 *  @param[in] window is the window that is going to be used for setting the icon.
 *  @param[in] path the source where the image is found.
 *
 *  @errors Window icon could not be setted
 */
void WindowSetIcon(Window* window, const char* path)
{
	GLFWimage* image = (GLFWimage*)malloc(sizeof(GLFWimage));
	image[0].pixels = stbi_load(path, &image[0].width, &image[0].height, NULL, STBI_rgb_alpha);
	glfwSetWindowIcon(window->window, 1, image);

	free(image);
}

/*! @brief return the CPU name
 *
 *  @errors the char array could not be correctly allocated
 */
static void window_get_system_info()
{
	int CPUInfo[4] = { -1 };
	unsigned   nExIds, i = 0;
	char* CPUBrandString = (char*)malloc(sizeof(char) * 0x40);

	// Get the information associated with each extended ID.
	__cpuid(CPUInfo, 0x80000000);
	nExIds = CPUInfo[0];
	for (i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);
		// Interpret CPU brand string
		if (i == 0x80000002)
			memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000003)
			memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000004)
			memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
	}
	memcpy(&CPU_Info, &CPUBrandString, sizeof(char**));

}

void WindowInit()
{
	if (!glfwInit())  // init glfw
	{
		printf("Could not create a GLFW context\n");
		exit(-1);
	}
	window_get_system_info();
}

/*! @brief Creates a new window with its context.
 *
 *  @param[in] width Defines the window width.
 *  @param[in] heigth Defines the window height.
 *  @param[in] title Defines the window title.
 *  @param[in] fullscreen Defines if the window is going to be fullscreen or not.
 *
 *  @return The allocated window.
 *
 *  @errors Context could not be created
 */
void NewWindow(Window* self, int width, int height, const char* title, bool fullscreen)
{
	assert(self != NULL);

	self->width = width;
	self->height = height;
	self->fullscreen = fullscreen;
	self->vidMode = (GLFWvidmode*)glfwGetVideoMode(glfwGetPrimaryMonitor());

	printf("Loading Window...\n");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	self->window = glfwCreateWindow(self->width, self->height, title, self->fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	if (self->window == NULL)
	{
		printf("Could not create a GLFW window context\n");
		return;
	}

	glfwSetWindowPos(self->window, (self->vidMode->width - self->width) / 2, (self->vidMode->height - self->height) / 2);
	glfwSetFramebufferSizeCallback(self->window, __GLFWwindowResizeCallback);
	glfwSetErrorCallback(__GLFWwindowErrCallback);

	NewInput(&self->input, self->window);

	glfwMakeContextCurrent(self->window);
	if (!gladLoadGL())
	{
		fprintf(stderr, "Could not create a OpenGL context\n");
		return;
	}
	
	// OpenGL Related:
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	

	printf(
		"%s\nOpenGL %s\nGLFW %s\n%s\n%s\n",
		CPU_Info,
		glGetString(GL_VERSION),
		glfwGetVersionString(),
		glGetString(GL_RENDERER),
		glGetString(GL_VENDOR)
	);
}

/*! @brief Carry out the processing of the window.
 *
 *  @param[in] self Is the window that is going to be used in the function.
 *
 */
void WindowPollEvents(Window* self)
{
	float currentTime = (float)glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	InputUpdate(&self->input);

	glFlush();
	glfwSwapBuffers(self->window);
	glfwPollEvents();

	glfwGetFramebufferSize(self->window, &self->width, &self->height);
}

/*! @brief Checks if the window is running and doesn't need to be closed.
 *
 *  @param[in] self Is the window that is going to be used in the function.
 *
 *  @return If the window is running or not.
 */
int WindowIsRunning(Window* self)
{
	WindowPollEvents(self);
	return !glfwWindowShouldClose(self->window);
}

/*! @brief Change the size of the window.
 *
 *  @param[in] self Is the window that is going to be used in the function.
 *  @param[in] width Is the new width of thw window.
 *  @param[in] height Is the new height of thw window.
 *
 */
void WindowSetSize(Window* self, int width, int height)
{
	glfwSetWindowSize(self->window, width, height);
	glfwSetWindowPos(self->window, (self->vidMode->width - width) / 2, (self->vidMode->height - height) / 2);
}

/*! @brief Finishes the window and free's its memory.
 *
 *  @param[in] self Is the window that is going to be used in the function.
 */
void WindowTerminate(Window* self) {

	InputTerminate(&self->input);
	glfwDestroyWindow(self->window);
}
