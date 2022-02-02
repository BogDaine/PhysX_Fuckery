#include "TheTime.h"
#include <GLFW/glfw3.h>


static double dt = 0;
static double lastTime = 0;

double TheTime::DetlaTime()
{
	return dt;
}

double TheTime::LastTime()
{
	return lastTime;
}

void TheTime::UpdateDeltaTime()
{
	double currentTime = glfwGetTime();
	dt = currentTime - lastTime;
	lastTime = currentTime;
}
