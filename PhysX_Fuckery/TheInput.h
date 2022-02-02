#pragma once
#include <GLFW/glfw3.h>
namespace TheInput
{
	extern double MouseDeltaX;
	extern double MouseDeltaY;
	extern float MouseSensitivity;
	extern bool MouseMoved;
	
	extern bool KeyPressed[360];
	extern bool KeyToggled[360];
	
	void MouseCallback(GLFWwindow* window, double xPos, double yPos);
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	bool MouseHasMoved();
}