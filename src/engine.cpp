#include "engine.h"
#include <iostream>
#include <memory>
#include "rendering/renderer.h"
#include "rendering/rendering_primitives.h"
#include "input/input.h"
#include "window/window.h"

namespace Poole 
{
    void TestMeshNoIndiciesSolidColor2()
    {
        Rendering::StaticMeshNoIndiciesSolidColor3_2DTransform meshNoInd;
        meshNoInd.m_verts = std::vector<Rendering::Vertex>{
            {-1.f,  -1.f, 0.0f},
            { 1.f,  -1.f, 0.0f},
            { 0.f,  1.f,  0.0f},
        };
        meshNoInd.m_color = Colors::Green<fcolor3>;
        Rendering::Renderer::Submit(std::move(meshNoInd));
    }


    void TestMeshNoIndiciesSolidColor()
    {
        Rendering::StaticMeshNoIndiciesSolidColor3_2DTransform meshNoInd;
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
        meshNoInd.m_color = Colors::Green<fcolor3>;
        Rendering::Renderer::Submit(std::move(meshNoInd));


        Rendering::StaticMeshNoIndiciesSolidColor3_2DTransform meshNoInd2;
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
        meshNoInd2.m_color = Colors::Pink<fcolor3>;
        Rendering::Renderer::Submit(std::move(meshNoInd2));

        Rendering::StaticMeshNoIndiciesSolidColor3_2DTransform meshNoInd3;
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
        meshNoInd3.m_color = Colors::Cyan<fcolor3>;
        Rendering::Renderer::Submit(std::move(meshNoInd3));
    }
    void TestMeshSolidColor()
    {
        Rendering::StaticMeshSolidColor3_2DTransform mesh;
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
        mesh.m_color = Colors::Red<fcolor3>;
        Rendering::Renderer::Submit(std::move(mesh));

        Rendering::StaticMeshSolidColor3_2DTransform mesh2;
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
        mesh2.m_color = Colors::Orange<fcolor3>;
        Rendering::Renderer::Submit(std::move(mesh2));
    }
    void TestMeshVertexColor()
    {
        Rendering::StaticMeshVertexColor3_2DTransform meshWithColor;
        meshWithColor.m_verts = std::vector<Rendering::VertexWithColor3>{
            {Rendering::Vertex{0.3f,   0.3f, 0.0f}, /*top right*/      Colors::Yellow<fcolor3>},
            {Rendering::Vertex{0.3f,  -0.3f, 0.0f}, /*bottom right*/   Colors::Green<fcolor3>},
            {Rendering::Vertex{-0.3f, -0.3f, 0.0f}, /*bottom left*/    Colors::White<fcolor3>},
            {Rendering::Vertex{-0.3f,  0.3f, 0.0f}, /*top left*/       Colors::Purple<fcolor3>},

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

        Rendering::StaticMeshVertexColor3_2DTransform meshWithColor2;
        meshWithColor2.m_verts = std::vector<Rendering::VertexWithColor3>{
            //{Rendering::Vertex{0.3f,   0.3f, 0.0f}, /*top right*/      Colors::Yellow<fColor3>},
            //{Rendering::Vertex{0.3f,  -0.3f, 0.0f}, /*bottom right*/   Colors::Green<fColor3>},
            //{Rendering::Vertex{-0.3f, -0.3f, 0.0f}, /*bottom left*/    Colors::White<fColor3>},
            //{Rendering::Vertex{-0.3f,  0.3f, 0.0f}, /*top left*/       Colors::Purple<fColor3>},

            {Rendering::Vertex{-0.7f,  0.9f, 0.0f}, /*top right*/      Colors::Orange<fcolor3>},
            {Rendering::Vertex{-0.7f,  0.7f, 0.0f}, /*bottom right*/   Colors::Cyan<fcolor3>},
            {Rendering::Vertex{-0.9f,  0.7f, 0.0f}, /*bottom left*/    Colors::Grey<fcolor3>},
            {Rendering::Vertex{-0.9f,  0.9f, 0.0f}, /*top left*/       Colors::Pink<fcolor3>},
        };
        meshWithColor2.m_indices = {
            0, 1, 3,   // first triangle
            1, 2, 3,    // second triangle

            //4+0, 4+1, 4+3,   // first triangle
            //4+1, 4+2, 4+3    // second triangle
        };
        Rendering::Renderer::Submit(std::move(meshWithColor2));
    }

    void TempPassTriangleData()
    {
        TestMeshNoIndiciesSolidColor2();
        //TestMeshNoIndiciesSolidColor();
        //TestMeshSolidColor();
        //TestMeshVertexColor();
    }

    void TempMoveTriangles()
    {
        return;
        Rendering::StaticMeshNoIndiciesSolidColor3_2DTransform* Mesh =
            Rendering::Renderer::GetMesh<Rendering::StaticMeshNoIndiciesSolidColor3_2DTransform>(0);
        
        static float P = 0.f;
        P += 0.01f;
        
        Mesh->m_position = { cosf(P/5)/3, sinf(P/5)/3 };
        Mesh->m_rotation = P * 0.01f;
        Mesh->m_scale = { (cosf(P/25)+2) / 2, (cosf(P/25)+2) / 2 };
        Mesh->m_shear = { 0.1, 0.1 };
    }

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
        TempPassTriangleData();

        //Loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            Input::Tick(window);
            Rendering::Renderer::Tick(window);
            Window::Tick();

            UpdateApp(0.f);
            TempMoveTriangles();
        }

        EndApp();
        Window::Close();
    }
}