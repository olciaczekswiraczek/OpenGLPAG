#include "Window.h"

Window::Window(int width, int height, char* name)
{
    Init(width, height, name);
    m_inputHandler.Init(m_window);
}

Window::~Window()
{
}

bool Window::isOpen()
{
    return glfwWindowShouldClose(m_window) == false;
}

GLFWwindow* Window::getWindow()
{
    return m_window;
}

bool Window::Init(int width, int height, char* name)
{
    // glfw: initialize and configure
       // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    m_window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (m_window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_window);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    int frameX, frameY;
    glfwGetFramebufferSize(m_window, &frameX, &frameY);

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, frameX, frameY);

    return true;
}
