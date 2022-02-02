#include "TheInput.h"
#include <math.h>
float TheInput::MouseSensitivity = 0.1;
double TheInput::MouseDeltaX;
double TheInput::MouseDeltaY;

bool TheInput::MouseMoved = false;
bool TheInput::KeyPressed[360];
bool TheInput::KeyToggled[360];

void TheInput::MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	MouseMoved = true;

	static double lastX;
	static double lastY;
	static bool FirstMouseMove = true;

	if (FirstMouseMove) {
		lastX = xPos;
		lastY = yPos;
		FirstMouseMove = false;
	}

	MouseDeltaX = xPos - lastX;
	MouseDeltaY = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	if (fabs(MouseDeltaX) <= 1e-6 && fabs(MouseDeltaY) <= 1e-6) {
		return;
	}
}

void TheInput::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
		KeyPressed[key] = true;
		KeyToggled[key] = !KeyToggled[key];
		break;

	case GLFW_RELEASE:
		KeyPressed[key] = false;
		break;
	}

}

bool TheInput::MouseHasMoved()
{
	return MouseMoved;
}
