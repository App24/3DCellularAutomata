#include "Events.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>

bool* Events::m_keys;
uint* Events::m_frames;
uint Events::m_current = 0;
float Events::m_deltaX = 0.0f;
float Events::m_deltaY = 0.0f;
float Events::m_x = 0.0f;
float Events::m_y = 0.0f;
bool Events::m_cursor_started = false;

#define _MOUSE_BUTTONS 1024

void Events::m_cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	if (m_cursor_started) {
		m_deltaX += xpos - m_x;
		m_deltaY += ypos - m_y;
	}
	else {
		m_cursor_started = true;
	}
	m_x = xpos;
	m_y = ypos;
}

void Events::m_mouse_button_callback(GLFWwindow* window, int button, int action, int mode) {
	if (action == GLFW_PRESS) {
		m_keys[_MOUSE_BUTTONS + button] = true;
		m_frames[_MOUSE_BUTTONS + button] = m_current;
	}
	else if (action == GLFW_RELEASE) {
		m_keys[_MOUSE_BUTTONS + button] = false;
		m_frames[_MOUSE_BUTTONS + button] = m_current;
	}
}

void Events::m_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		m_keys[key] = true;
		m_frames[key] = m_current;
	}
	else if (action == GLFW_RELEASE) {
		m_keys[key] = false;
		m_frames[key] = m_current;
	}
}

void Events::m_window_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	Window::setWidth(width);
	Window::setHeight(height);
}

int Events::initialise() {
	GLFWwindow* window = Window::getWindow();
	m_keys = new bool[1032];
	m_frames = new uint[1032];

	memset(m_keys, false, 1032 * sizeof(bool));
	memset(m_frames, 0, 1032 * sizeof(uint));

	glfwSetKeyCallback(window, m_key_callback);
	glfwSetMouseButtonCallback(window, m_mouse_button_callback);
	glfwSetCursorPosCallback(window, m_cursor_position_callback);
	glfwSetWindowSizeCallback(window, m_window_size_callback);
	return 0;
}

void Events::finalise() {
	delete[] m_keys;
	delete[] m_frames;
}

bool Events::keyHeld(int keycode) {
	if (keycode < 0 || keycode >= _MOUSE_BUTTONS)
		return false;
	return m_keys[keycode];
}

bool Events::keyPressed(int keycode) {
	if (keycode < 0 || keycode >= _MOUSE_BUTTONS)
		return false;
	return m_keys[keycode] && m_frames[keycode] == m_current;
}

bool Events::buttonClicked(int button) {
	int index = _MOUSE_BUTTONS + button;
	return m_keys[index];
}

float Events::getMouseX()
{
	return m_x;
}

float Events::getMouseY()
{
	return m_y;
}

bool Events::buttonHeld(int button) {
	int index = _MOUSE_BUTTONS + button;
	return m_keys[index] && m_frames[index] == m_current;
}

void Events::pollEvents() {
	m_current++;
	m_deltaX = 0.0f;
	m_deltaY = 0.0f;
	glfwPollEvents();
}