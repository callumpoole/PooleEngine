#include "engine.h"
#include <iostream>
#include <memory>
#include "rendering/renderer.h"
#include "rendering/rendering_primitives.h"
#include "input/input.h"
#include "window/window.h"

namespace Poole 
{
    void TempPassTriangleData()
    {
        //Rendering::MeshBasicNoIndiciesSolidColor3 meshNoInd;
        //meshNoInd.m_mesh.m_verts = std::vector<Rendering::Vertex>{
        //    {-0.5f,  -0.5f,  0.0f},
        //    { 0.5f,  -0.5f,  0.0f},
        //    { 0.0f,   0.5f,  0.0f},
        //
        //    {-0.75f, -0.75f, 0.0f},
        //    {-0.25f, -0.75f, 0.0f},
        //    {-0.5f,  -0.5f,  0.0f},
        //
        //    {0.75f, 0.75f, 0.0f},
        //    {0.25f, 0.75f, 0.0f},
        //    {0.5f,  0.5f,  0.0f},
        //};
        //meshNoInd.m_color = Colors::Green<LinearColor3>;
        //Rendering::Renderer::Submit(std::move(meshNoInd));


        Rendering::MeshBasicSolidColor3 mesh;
        mesh.m_mesh.m_verts = std::vector<Rendering::Vertex>{
            {0.9f,   0.3f, 0.0f}, // top right
            {0.9f,  -0.3f, 0.0f}, // bottom right
            {0.6f,  -0.3f, 0.0f}, // bottom left
            {0.6f,   0.3f, 0.0f}, // top left
        
            { 0.9f,  0.9f, 0.0f}, // top right
            { 0.9f,  0.7f, 0.0f}, // bottom right
            { 0.7f,  0.7f, 0.0f}, // bottom left
            { 0.7f,  0.9f, 0.0f}, // top left
        };
        mesh.m_mesh.m_indices = {
            0, 1, 3,   // first triangle
            1, 2, 3,    // second triangle
        
            4+0, 4+1, 4+3,   // first triangle
            4+1, 4+2, 4+3    // second triangle
        };
        mesh.m_color = Colors::Blue<LinearColor3>;
        Rendering::Renderer::Submit(std::move(mesh));

        //Rendering::MeshBasicVertexColor3 mesh;
        //mesh.m_verts = std::vector<Rendering::VertexWithColor3>{
        //    {0.3f,   0.3f, 0.0f, /*top right*/      1.0f, 1.0f, 0.0f}, // yellow
        //    {0.3f,  -0.3f, 0.0f, /*bottom right*/   0.0f, 1.0f, 0.0f}, // green
        //    {-0.3f, -0.3f, 0.0f, /*bottom left*/    1.0f, 1.0f, 1.0f}, // white
        //    {-0.3f,  0.3f, 0.0f, /*top left*/       1.0f, 0.0f, 0.0f}, // purple
        //
        //    {-0.7f,  0.9f, 0.0f, /*top right*/      1.0f, 0.5f, 0.0f}, // orange
        //    {-0.7f,  0.7f, 0.0f, /*bottom right*/   0.0f, 1.0f, 1.0f}, // cyan
        //    {-0.9f,  0.7f, 0.0f, /*bottom left*/    0.2f, 0.2f, 0.2f}, // dark grey
        //    {-0.9f,  0.9f, 0.0f, /*top left*/       0.5f, 0.5f, 0.5f}, // medium grey
        //};
        //mesh.m_indices = {
        //    0, 1, 3,   // first triangle
        //    1, 2, 3,    // second triangle
        //
        //    4+0, 4+1, 4+3,   // first triangle
        //    4+1, 4+2, 4+3    // second triangle
        //};
        //Rendering::Renderer::Submit(std::move(mesh));
    }




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

        TempPassTriangleData();

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