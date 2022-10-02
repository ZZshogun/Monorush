#include "Input.h"

GLFWwindow* Input::mainWindow = NULL;
GLuint Input::keyCode[GLFW_KEY_LAST + 1] = { Input::KeyState::KEY_RELEASE };
GLuint Input::mouseCode[GLFW_MOUSE_BUTTON_LAST + 1] = { Input::KeyState::KEY_RELEASE };

void Input::SetWindowInput(GLFWwindow* window) {
	mainWindow = window;
}

void Input::ScanKey(GLFWwindow* window) {
	SetWindowInput(window);

	for (int i = 32; i <= GLFW_KEY_LAST; i++) {
		if (glfwGetKeyScancode(i) == -1) continue;

		int action = glfwGetKey(window, i);

		if ((keyCode[i] == KEY_RELEASE || keyCode[i] == KEY_UP) && action == GLFW_PRESS) 
			keyCode[i] = KEY_DOWN;

		else if (keyCode[i] == KEY_DOWN && action == GLFW_PRESS) 
			keyCode[i] = KEY_STAY;

		else if ((keyCode[i] == KEY_STAY || keyCode[i] == KEY_DOWN) && action == GLFW_RELEASE) 
			keyCode[i] = KEY_UP;

		else if (action == GLFW_RELEASE) 
			keyCode[i] = KEY_RELEASE;
	}
}

void Input::ScanMouse(GLFWwindow* window) {
	SetWindowInput(window);

	for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; i++) {
		int action = glfwGetMouseButton(window, i);

		if ((mouseCode[i] == KEY_RELEASE || mouseCode[i] == KEY_UP) && action == GLFW_PRESS)
			mouseCode[i] = KEY_DOWN;

		else if (mouseCode[i] == KEY_DOWN && action == GLFW_PRESS)
			mouseCode[i] = KEY_STAY;

		else if ((mouseCode[i] == KEY_STAY || mouseCode[i] == KEY_DOWN) && action == GLFW_RELEASE)
			mouseCode[i] = KEY_UP;

		else if (action == GLFW_RELEASE)
			mouseCode[i] = KEY_RELEASE;
	}
}

bool Input::GetKey(GLenum GLFW_KEY) {
	if (!mainWindow || GLFW_KEY == GLFW_KEY_UNKNOWN) return false;
	return keyCode[GLFW_KEY] == KeyState::KEY_STAY || keyCode[GLFW_KEY] == KeyState::KEY_DOWN;
}

bool Input::GetKeyDown(GLenum GLFW_KEY) {
	if (!mainWindow || GLFW_KEY == GLFW_KEY_UNKNOWN) return false;
	return keyCode[GLFW_KEY] == KeyState::KEY_DOWN;
}

bool Input::GetKeyUp(GLenum GLFW_KEY) {
	if (!mainWindow || GLFW_KEY == GLFW_KEY_UNKNOWN) return false;
	return keyCode[GLFW_KEY] == KeyState::KEY_UP;
}

glm::vec2 Input::MousePosition() {
	if (!mainWindow) return glm::vec2(-1, -1);

	double x, y;
	glfwGetCursorPos(mainWindow, &x, &y);
	return glm::vec2((float)x, (float)y);
}

bool Input::GetMouse(GLenum GLFW_MOUSE_BUTTON) {
	if (!mainWindow) return false;
	return mouseCode[GLFW_MOUSE_BUTTON] == KeyState::KEY_STAY || mouseCode[GLFW_MOUSE_BUTTON] == KeyState::KEY_DOWN;
}

bool Input::GetMouseDown(GLenum GLFW_MOUSE_BUTTON) {
	if (!mainWindow) return false;
	return mouseCode[GLFW_MOUSE_BUTTON] == KeyState::KEY_DOWN;
}

bool Input::GetMouseUp(GLenum GLFW_MOUSE_BUTTON) {
	if (!mainWindow) return false;
	return mouseCode[GLFW_MOUSE_BUTTON] == KeyState::KEY_UP;
}

glm::vec2 Input::WASDAxis() {
	if (!mainWindow) return glm::vec2(-1, -1);

	glm::vec2 dir(0, 0);

	if (GetKey(GLFW_KEY_W)) dir.y += 1;
	if (GetKey(GLFW_KEY_S)) dir.y -= 1;
	if (GetKey(GLFW_KEY_A)) dir.x -= 1;
	if (GetKey(GLFW_KEY_D)) dir.x += 1;

	return dir;
}