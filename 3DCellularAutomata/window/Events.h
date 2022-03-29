#pragma once

#include "Window.h"

typedef unsigned int uint;

class Events {
public:
	static int initialise();
	static void finalise();
	static void pollEvents();

	static bool keyHeld(int keycode);
	static bool keyPressed(int keycode);

	static bool buttonHeld(int button);
	static bool buttonClicked(int button);

	static float getMouseX();
	static float getMouseY();
private:
	static bool* m_keys;
	static uint* m_frames;
	static uint m_current;
	static float m_deltaX;
	static float m_deltaY;
	static float m_x;
	static float m_y;
	static bool m_cursor_started;

	static void m_cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void m_mouse_button_callback(GLFWwindow* window, int button, int action, int mode);
	static void m_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void m_window_size_callback(GLFWwindow* window, int width, int height);
};