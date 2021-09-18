#include "window.h"
#include <iostream>

namespace Poole
{
    std::string_view Window::m_windowName;
    glm::uvec2 Window::m_windowSize = {};

    void FramebufferSizeCallback(GLFWwindow* /*window*/, int width, int height)
    {
        //#todo: Can't as it's private and this is a free function
        //Window::m_windowSize = size;
        glViewport(0, 0, width, height);
    }

    /*static*/ GLFWwindow* Window::Init(const char* windowName, glm::uvec2 size)
    {
        //Create a windowed mode window and its OpenGL context
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        GLFWwindow* window = glfwCreateWindow(size.x, size.y, windowName, NULL, NULL);
        if (!window)
        {
            std::cout << "Failed to Create Window\n";
            glfwTerminate();
            return nullptr;
        }
        m_windowName = windowName;
        m_windowSize = size;

        //Make the window's context current
        glfwMakeContextCurrent(window);

        return window;
    }

    /*static*/ void Window::SetupPostGlad(GLFWwindow* window)
    {
        //setup current window size
        glViewport(0, 0, m_windowSize.x, m_windowSize.y);
        //Listen to resize events
        glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    }

    /*static*/ void Window::Tick()
    {
        //Poll for and process events
        glfwPollEvents();
    }

    /*static*/ void Window::Close()
    {
        glfwTerminate();
    }
}