#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "ShaderProgram.h"

class InputHandler
{
public:
	InputHandler();
	~InputHandler();

	void Init(GLFWwindow* window);

	static void KeyInputCallback(GLFWwindow* window, int key, int scancode, int state, int mods);
};

