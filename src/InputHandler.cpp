#include "InputHandler.h"

InputHandler::InputHandler()
{
}

InputHandler::~InputHandler()
{
}

void InputHandler::Init(GLFWwindow* window)
{
	glfwSetKeyCallback(window, KeyInputCallback);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void InputHandler::KeyInputCallback(GLFWwindow* window, int key, int scancode, int state, int mods)
{
	if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (key == GLFW_KEY_R && state == GLFW_PRESS);
	{
		ShaderProgram::currentProgram->Refresh();
	}
	
}
