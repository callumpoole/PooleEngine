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
        Rendering::MeshBasicNoIndiciesSolidColor3 meshNoInd;
        meshNoInd.m_verts = std::vector<Rendering::Vertex>{
            {-0.5f,  -0.5f,  0.0f},
            { 0.5f,  -0.5f,  0.0f},
            { 0.0f,   0.5f,  0.0f},
        
            //{-0.75f, -0.75f, 0.0f},
            //{-0.25f, -0.75f, 0.0f},
            //{-0.5f,  -0.5f,  0.0f},
            
            //{0.75f, 0.75f, 0.0f},
            //{0.25f, 0.75f, 0.0f},
            //{0.5f,  0.5f,  0.0f},
        };
        meshNoInd.m_color = Colors::Green<fColor3>;
        Rendering::Renderer::Submit(std::move(meshNoInd));

        Rendering::MeshBasicNoIndiciesSolidColor3 meshNoInd2;
        meshNoInd2.m_verts = std::vector<Rendering::Vertex>{
            //{-0.5f,  -0.5f,  0.0f},
            //{ 0.5f,  -0.5f,  0.0f},
            //{ 0.0f,   0.5f,  0.0f},

            {-0.75f, -0.75f, 0.0f},
            {-0.25f, -0.75f, 0.0f},
            {-0.5f,  -0.5f,  0.0f},

            //{0.75f, 0.75f, 0.0f},
            //{0.25f, 0.75f, 0.0f},
            //{0.5f,  0.5f,  0.0f},
        };
        meshNoInd2.m_color = Colors::Pink<fColor3>;
        Rendering::Renderer::Submit(std::move(meshNoInd2));

        Rendering::MeshBasicNoIndiciesSolidColor3 meshNoInd3;
        meshNoInd3.m_verts = std::vector<Rendering::Vertex>{
            //{-0.5f,  -0.5f,  0.0f},
            //{ 0.5f,  -0.5f,  0.0f},
            //{ 0.0f,   0.5f,  0.0f},

            //{-0.75f, -0.75f, 0.0f},
            //{-0.25f, -0.75f, 0.0f},
            //{-0.5f,  -0.5f,  0.0f},

            {0.75f, 0.75f, 0.0f},
            {0.25f, 0.75f, 0.0f},
            {0.5f,  0.5f,  0.0f},
        };
        meshNoInd3.m_color = Colors::Cyan<fColor3>;
        Rendering::Renderer::Submit(std::move(meshNoInd3));




        Rendering::MeshBasicSolidColor3 mesh;
        mesh.m_verts = std::vector<Rendering::Vertex>{
            {0.9f,   0.3f, 0.0f}, // top right
            {0.9f,  -0.3f, 0.0f}, // bottom right
            {0.6f,  -0.3f, 0.0f}, // bottom left
            {0.6f,   0.3f, 0.0f}, // top left
        
            //{ 0.9f,  0.9f, 0.0f}, // top right
            //{ 0.9f,  0.7f, 0.0f}, // bottom right
            //{ 0.7f,  0.7f, 0.0f}, // bottom left
            //{ 0.7f,  0.9f, 0.0f}, // top left
        };
        mesh.m_indices = {
            0, 1, 3,   // first triangle
            1, 2, 3,    // second triangle
        
            //4+0, 4+1, 4+3,   // first triangle
            //4+1, 4+2, 4+3    // second triangle
        };
        mesh.m_color = Colors::Red<fColor3>;
        Rendering::Renderer::Submit(std::move(mesh));

        Rendering::MeshBasicSolidColor3 mesh2;
        mesh2.m_verts = std::vector<Rendering::Vertex>{
            //{0.9f,   0.3f, 0.0f}, // top right
            //{0.9f,  -0.3f, 0.0f}, // bottom right
            //{0.6f,  -0.3f, 0.0f}, // bottom left
            //{0.6f,   0.3f, 0.0f}, // top left

            { 0.9f,  0.9f, 0.0f}, // top right
            { 0.9f,  0.7f, 0.0f}, // bottom right
            { 0.7f,  0.7f, 0.0f}, // bottom left
            { 0.7f,  0.9f, 0.0f}, // top left
        };
        mesh2.m_indices = {
            0, 1, 3,   // first triangle
            1, 2, 3,    // second triangle

            //4+0, 4+1, 4+3,   // first triangle
            //4+1, 4+2, 4+3    // second triangle
        };
        mesh2.m_color = Colors::Orange<fColor3>;
        Rendering::Renderer::Submit(std::move(mesh2));
       



        Rendering::MeshBasicVertexColor3 meshWithColor;
        meshWithColor.m_verts = std::vector<Rendering::VertexWithColor3>{
            {Rendering::Vertex{0.3f,   0.3f, 0.0f}, /*top right*/      Colors::Yellow<fColor3>},
            {Rendering::Vertex{0.3f,  -0.3f, 0.0f}, /*bottom right*/   Colors::Green<fColor3>},
            {Rendering::Vertex{-0.3f, -0.3f, 0.0f}, /*bottom left*/    Colors::White<fColor3>},
            {Rendering::Vertex{-0.3f,  0.3f, 0.0f}, /*top left*/       Colors::Purple<fColor3>},
                                                                       
            //{Rendering::Vertex{-0.7f,  0.9f, 0.0f}, /*top right*/      Colors::Orange<fColor3>},
            //{Rendering::Vertex{-0.7f,  0.7f, 0.0f}, /*bottom right*/   Colors::Cyan<fColor3>},
            //{Rendering::Vertex{-0.9f,  0.7f, 0.0f}, /*bottom left*/    Colors::Grey<fColor3>}, 
            //{Rendering::Vertex{-0.9f,  0.9f, 0.0f}, /*top left*/       Colors::Pink<fColor3>},
        };
        meshWithColor.m_indices = {
            0, 1, 3,   // first triangle
            1, 2, 3,    // second triangle
        
            //4+0, 4+1, 4+3,   // first triangle
            //4+1, 4+2, 4+3    // second triangle
        };
        Rendering::Renderer::Submit(std::move(meshWithColor));

        Rendering::MeshBasicVertexColor3 meshWithColor2;
        meshWithColor2.m_verts = std::vector<Rendering::VertexWithColor3>{
            //{Rendering::Vertex{0.3f,   0.3f, 0.0f}, /*top right*/      Colors::Yellow<fColor3>},
            //{Rendering::Vertex{0.3f,  -0.3f, 0.0f}, /*bottom right*/   Colors::Green<fColor3>},
            //{Rendering::Vertex{-0.3f, -0.3f, 0.0f}, /*bottom left*/    Colors::White<fColor3>},
            //{Rendering::Vertex{-0.3f,  0.3f, 0.0f}, /*top left*/       Colors::Purple<fColor3>},

            {Rendering::Vertex{-0.7f,  0.9f, 0.0f}, /*top right*/      Colors::Orange<fColor3>},
            {Rendering::Vertex{-0.7f,  0.7f, 0.0f}, /*bottom right*/   Colors::Cyan<fColor3>},
            {Rendering::Vertex{-0.9f,  0.7f, 0.0f}, /*bottom left*/    Colors::Grey<fColor3>},
            {Rendering::Vertex{-0.9f,  0.9f, 0.0f}, /*top left*/       Colors::Pink<fColor3>},
        };
        meshWithColor2.m_indices = {
            0, 1, 3,   // first triangle
            1, 2, 3,    // second triangle

            //4+0, 4+1, 4+3,   // first triangle
            //4+1, 4+2, 4+3    // second triangle
        };
        Rendering::Renderer::Submit(std::move(meshWithColor2));
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