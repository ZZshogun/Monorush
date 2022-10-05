#ifndef INPUT_CLASS_H
#define INPUT_CLASS_H

#include "Core.h"

class Input {

	enum KeyState {
		KEY_RELEASE,
		KEY_DOWN,
		KEY_STAY,
		KEY_UP,
	};

	static GLFWwindow* mainWindow;

	static GLuint keyCode[GLFW_KEY_LAST + 1];
	static GLuint mouseCode[GLFW_MOUSE_BUTTON_LAST + 1];
	static glm::vec2 mouseScrollDelta;

public:
	static void SetWindowInput(GLFWwindow* window);
	static void ScanKey(GLFWwindow* window);
	static void ScanMouse(GLFWwindow* window);
	static void ScanMouseScroll(GLFWwindow* window, double xoffset, double yoffset);

	static void ClearInputBuffer();

	static bool GetKey(GLenum GLFW_KEY);
	static bool GetKeyDown(GLenum GLFW_KEY);
	static bool GetKeyUp(GLenum GLFW_KEY);

	static glm::vec2 MousePosition();
	static glm::vec2 MouseScrollDelta();

	static bool GetMouse(GLenum GLFW_MOUSE_BUTTON);
	static bool GetMouseDown(GLenum GLFW_MOUSE_BUTTON);
	static bool GetMouseUp(GLenum GLFW_MOUSE_BUTTON);

	static glm::vec2 WASDAxis();
};


#endif