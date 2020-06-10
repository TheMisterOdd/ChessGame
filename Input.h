#pragma once

#include "Window.h"

#include <stdbool.h>

typedef struct
{
	GLFWwindow* window;
	bool* keys;
	bool* mouseButtons;

}Input;

void NewInput(Input* self, GLFWwindow* window)
{
	self->window = window;
	self->keys = (bool*)malloc(sizeof(bool) * GLFW_KEY_LAST);
	self->mouseButtons = (bool*)malloc(sizeof(bool) * GLFW_MOUSE_BUTTON_LAST);
}

bool InputIsKeyDown(Input* self, uint16_t key)
{
	return glfwGetKey(self->window, key) == 1;
}

bool InputIsKeyPressed(Input* self, uint16_t key)
{
	return (InputIsKeyDown(self, key) && !self->keys[key]);
}

bool InputIsKeyReleased(Input* self, uint16_t key)
{
	return (!InputIsKeyDown(self, key) && self->keys[key]);
}

bool InputIsMouseButtonDown(Input* self, uint8_t button)
{
	return glfwGetMouseButton(self->window, button) == 1;
}

bool InputIsMouseButtonPressed(Input* self, uint8_t button)
{
	return (InputIsMouseButtonDown(self, button) && !self->mouseButtons[button]);
}

bool InputIsMouseButtonReleased(Input* self, uint8_t button)
{
	return (!InputIsMouseButtonDown(self, button) && self->mouseButtons[button]);
}

void InputUpdate(Input* self)
{
	for (uint16_t i = 32; i < GLFW_KEY_LAST; i++)
	{
		self->keys[i] = InputIsKeyDown(self, i);
	}

	for (uint8_t i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
	{
		self->mouseButtons[i] = InputIsMouseButtonDown(self, i);
	}
}

void InputGetMousePos(Input* self, double* x, double* y)
{
	glfwGetCursorPos(self->window, x, y);
}

void InputTerminate(Input* self)
{
	free(self->keys);
	free(self->mouseButtons);
}
