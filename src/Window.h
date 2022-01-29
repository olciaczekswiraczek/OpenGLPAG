#pragma once
#include "InputHandler.h"
#include <iostream>
#include <Camera.h>

class Window
{
public:
	Window(int width, int height, char* name);
	~Window();

	bool isOpen();
	GLFWwindow* getWindow();


private:

	bool Init(int width, int height, char* name);
	GLFWwindow* m_window;

	InputHandler m_inputHandler;
	Camera camera;
};
