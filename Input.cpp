#include "Input.h"

GLFWwindow* Input::mainWindow = NULL;
GLuint Input::keyCode[GLFW_KEY_LAST + 1] = { Input::KeyState::KEY_RELEASE };
GLuint Input::mouseCode[GLFW_MOUSE_BUTTON_LAST + 1] = { Input::KeyState::KEY_RELEASE };
glm::vec2 Input::mouseScrollDelta = glm::vec2(0, 0);

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

void Input::ScanMouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
	SetWindowInput(window);

	mouseScrollDelta += glm::vec2(xoffset, yoffset);
}

void Input::ClearInputBuffer() {
	if (!mainWindow) return;

	mouseScrollDelta = glm::vec2(0, 0);
}

bool Input::GetKey(Key key) {
	if (!mainWindow || key == GLFW_KEY_UNKNOWN) return false;
	return keyCode[key] == KeyState::KEY_STAY || keyCode[key] == KeyState::KEY_DOWN;
}

bool Input::GetKeyDown(Key key) {
	if (!mainWindow || key == GLFW_KEY_UNKNOWN) return false;
	return keyCode[key] == KeyState::KEY_DOWN;
}

bool Input::GetKeyUp(Key key) {
	if (!mainWindow || key == GLFW_KEY_UNKNOWN) return false;
	return keyCode[key] == KeyState::KEY_UP;
}

glm::vec2 Input::MousePosition() {
	if (!mainWindow) return glm::vec2(-1, -1);

	double x, y;
	glfwGetCursorPos(mainWindow, &x, &y);
	return glm::vec2((float)x, (float)y);
}

glm::vec2 Input::MouseScrollDelta() {
	if (!mainWindow) return glm::vec2(-1, -1);
	return mouseScrollDelta;
}

bool Input::GetMouse(Button button) {
	if (!mainWindow) return false;
	return (mouseCode[button] == KeyState::KEY_STAY || mouseCode[button] == KeyState::KEY_DOWN) && !UI::onEvents;
}

bool Input::GetMouseDown(Button button) {
	if (!mainWindow) return false;
	return mouseCode[button] == KeyState::KEY_DOWN && !UI::onEvents;
}

bool Input::GetMouseUp(Button button) {
	if (!mainWindow) return false;
	return mouseCode[button] == KeyState::KEY_UP;
}

glm::vec2 Input::WASDAxis() {
	if (!mainWindow) return glm::vec2(-1, -1);

	glm::vec2 dir(0, 0);

	if (GetKey(Key::W)) dir.y += 1;
	if (GetKey(Key::S)) dir.y -= 1;
	if (GetKey(Key::A)) dir.x -= 1;
	if (GetKey(Key::D)) dir.x += 1;

	return dir;
}