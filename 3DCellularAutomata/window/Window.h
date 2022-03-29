#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
	static int initialise(int width, int height, const char* title);
	static void terminate();

	static void swapBuffers();

	static bool isShouldClose();
	static void setShouldClose(bool value);

	static GLFWwindow* getWindow();
	static int getWidth();
	static int getHeight();

	static void setWidth(int value);
	static void setHeight(int value);
private:
	static GLFWwindow* m_window;
	static int m_width;
	static int m_height;
};