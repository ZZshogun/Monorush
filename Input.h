#ifndef INPUT_CLASS_H
#define INPUT_CLASS_H

#include "Core.h"

enum Key {
	Unknown = -1,
	Space = 32,
	Apostrope = 39,  /* ' */
	Comma = 44,  /* , */
	Minus = 45,  /* - */
	Period = 46,  /* . */
	Slash = 47,  /* / */
	Zero = 48,
	One = 49,
	Two = 50,
	Three = 51,
	Four = 52,
	Five = 53,
	Six = 54,
	Seven = 55,
	Eight = 56,
	Nine = 57,
	Semicolon = 59,  /* ; */
	Equal = 61,  /* = */
	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,
	Left_Bracket = 91,  /* [ */
	BackSlash = 92,  /* \ */
	Right_Bracket = 93,  /* ] */
	Grave_Accents = 96,  /* ` */
	World_1 = 161, /* non-US #1 */
	World_2 = 162, /* non-US #2 */
	Escape = 256,
	Enter = 257,
	Tab = 258,
	Backspace = 259,
	Insert = 260,
	Delete = 261,
	Right = 262,
	Left = 263,
	Down = 264,
	Up = 265,
	Page_Up = 266,
	Page_Down = 267,
	Home = 268,
	End = 269,
	Caps_lock = 280,
	Scroll_Lock = 281,
	Num_lock = 282,
	Print_Screen = 283,
	Pause = 284,
	F1 = 290,
	F2 = 291,
	F3 = 292,
	F4 = 293,
	F5 = 294,
	F6 = 295,
	F7 = 296,
	F8 = 297,
	F9 = 298,
	F10 = 299,
	F11 = 300,
	F12 = 301,
	F13 = 302,
	F14 = 303,
	F15 = 304,
	F16 = 305,
	F17 = 306,
	F18 = 307,
	F19 = 308,
	F20 = 309,
	F21 = 310,
	F22 = 311,
	F23 = 312,
	F24 = 313,
	F25 = 314,
	Keypad_0 = 320,
	Keypad_1 = 321,
	Keypad_2 = 322,
	Keypad_3 = 323,
	Keypad_4 = 324,
	Keypad_5 = 325,
	Keypad_6 = 326,
	Keypad_7 = 327,
	Keypad_8 = 328,
	Keypad_9 = 329,
	Keypad_Dot = 330,
	Keypad_Divide = 331,
	Keypad_Multiply = 332,
	Keypad_Substract = 333,
	Keypad_Add = 334,
	Keypad_Enter = 335,
	Keypad_Equal = 336,
	Left_Shift = 340,
	Left_Control = 341,
	Left_Alt = 342,
	Left_Super = 343,
	Right_SHIFT = 344,
	Right_Control = 345,
	Right_Alt = 346,
	Right_Super = 347,
	Menu = 348,
};

enum Button {
	Mouse_1      = 0,
	Mouse_2      = 1,
	Mouse_3      = 2,
	Mouse_4      = 3,
	Mouse_5      = 4,
	Mouse_6      = 5,
	Mouse_7      = 6,
	Mouse_8      = 7,
	Mouse_LEFT   = 0,
	Mouse_RIGHT  = 1,
	Mouse_MIDDLE = 2,
};

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

	static bool GetKey(Key key);
	static bool GetKeyDown(Key key);
	static bool GetKeyUp(Key key);

	static glm::vec2 MousePosition();
	static glm::vec2 MouseScrollDelta();

	static bool GetMouse(Button button);
	static bool GetMouseDown(Button button);
	static bool GetMouseUp(Button button);

	static glm::vec2 WASDAxis();
};


#endif