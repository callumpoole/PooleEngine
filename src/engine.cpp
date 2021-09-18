#include "engine.h"
#include <iostream>
#include <memory>
#include "rendering/renderer.h"
#include "rendering/rendering_primitives.h"
#include "input/input.h"
#include "window/window.h"

namespace Poole 
{
    Engine::Engine(const char* windowName, glm::uvec2 size)
    {
        //Initialize the library GLFW
        if (!glfwInit())
        {
            std::cout << "Failed to Init GLFW\n";
            return;
        }

        GLFWwindow* window = Window::Init(windowName, size);
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

        //TEMP
        Rendering::MeshBasicNoIndiciesSolidColor3 mesh;
        mesh.m_mesh.m_verts = std::vector<Rendering::Vertex>{
            {-0.5f,  -0.5f,  0.0f},
            { 0.5f,  -0.5f,  0.0f},
            { 0.0f,   0.5f,  0.0f},
            {-0.75f, -0.75f, 0.0f},
            {-0.25f, -0.75f, 0.0f},
            {-0.5f,  -0.5f,  0.0f},
        };
        Rendering::Renderer::Submit(std::move(mesh));


        //Loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            Input::Tick(window);
            Rendering::Renderer::Tick(window);
            Window::Tick();
        }
    
        Window::Close();
    }
}