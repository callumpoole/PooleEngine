#include "window.h"
#include <iostream>

namespace Poole
{
    GLFWwindow* Window::m_WindowInstance;
    std::vector<std::function<void(int width, int height)>> Window::m_OnResize;
    std::string Window::m_OriginalTitle;
    std::string Window::m_Title;
    uvec2 Window::m_WindowSize = {};

    void Window::FramebufferSizeCallback(GLFWwindow* /*window*/, int width, int height)
    {
        //#todo: Can't as it's private and this is a free function
        Window::m_WindowSize = uvec2(width, height);
        glViewport(0, 0, width, height);

        //Broadcast
        for (auto& Func : m_OnResize)
        {
            Func(width, height);
        }
    }

    /*static*/ GLFWwindow* Window::Init(const char* title, uvec2 size)
    {
        //Create a windowed mode window and its OpenGL context
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        GLFWwindow* window = glfwCreateWindow(size.x, size.y, title, NULL, NULL);
        if (!window)
        {
            std::cout << "Failed to Create Window\n";
            glfwTerminate();
            return nullptr;
        }
        m_OriginalTitle = title;
        m_Title = title;
        m_WindowSize = size;

        //Make the window's context current
        glfwMakeContextCurrent(window);

        m_WindowInstance = window;
        return window;
    }

    /*static*/ void Window::SetupPostGlad(GLFWwindow* window)
    {
        //setup current window size
        glViewport(0, 0, m_WindowSize.x, m_WindowSize.y);
        //Listen to resize events
        glfwSetFramebufferSizeCallback(window, 
            [](GLFWwindow* window, int width, int height)
            {
                FramebufferSizeCallback(window, width, height);
            });
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

    /*static*/ void Window::SetWindowTitle(const char* windowName)
    {
        glfwSetWindowTitle(m_WindowInstance, windowName);
    }
}
