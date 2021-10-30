#include "engine.h"
#include <iostream>
#include <memory>
#include "rendering/renderer.h"
#include "rendering/rendering_primitives.h"
#include "input/input.h"
#include "window/window.h"

namespace Poole 
{
    Engine::Engine()
        : m_RunData({ "Unamed Window", uvec2(640, 480) })
    {
    }

    Engine::Engine(const char* windowName, uvec2 size)
        : m_RunData({ windowName, size })
    {
    }

    void Engine::Run()
    {
        //Initialize the library GLFW
        if (!glfwInit())
        {
            std::cout << "Failed to Init GLFW\n";
            return;
        }

        GLFWwindow* window = Window::Init(m_RunData.windowName, m_RunData.size);
        if (!window)
        {
            return;
        }

        //Init Glad
        if (!gladLoadGL())
        {
            std::cout << "Failed to initialize OpenGL context\n";
            return;
        }
        if (!GLAD_GL_VERSION_3_0)
        {
            std::cout << "Open GL version too low!\n";
            return;
        }

        Window::SetupPostGlad(window);
        Input::Init(window);
        Rendering::Renderer::Init();


        BeginApp();

        //Loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            Input::Tick(window);

            Rendering::Renderer::BeginScene();

            UpdateApp(0.f);

            Rendering::Renderer::RenderScene();
            Rendering::Renderer::EndScene(window);

            Window::Tick();
        }

        EndApp();
        Window::Close();
    }
}