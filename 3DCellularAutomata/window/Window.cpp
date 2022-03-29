#include "Window.h"
#include <iostream>

GLFWwindow* Window::m_window;
int Window::m_width;
int Window::m_height;

int Window::initialise(int width, int height, const char* title)
{
	glfwInit();

	m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (m_window == nullptr) {
		std::cout << "Could not create window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(m_window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	glViewport(0, 0, width, height);

	glClearColor(0.0f, 0.0f, 0.0f, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSwapInterval(0);

	m_width = width;
	m_height = height;
}

void Window::terminate()
{
	glfwTerminate();
}

void Window::swapBuffers()
{
	glfwSwapBuffers(m_window);
}

bool Window::isShouldClose()
{
	return glfwWindowShouldClose(m_window);
}

void Window::setShouldClose(bool value)
{
	glfwSetWindowShouldClose(m_window, value);
}

GLFWwindow* Window::getWindow()
{
	return m_window;
}

int Window::getWidth()
{
	return m_width;
}

int Window::getHeight()
{
	return m_height;
}

void Window::setWidth(int value)
{
	m_width = value;
}

void Window::setHeight(int value)
{
	m_height = value;
}
