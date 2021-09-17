#include "engine.h"
#include <iostream>
#include "rendering/renderer.h"
#include "rendering/shader_loader.h"
#include "rendering/shader_literals/shader_literals.h"

namespace Poole {

    //This will identify our vertex buffer
    GLuint m_vertexbuffer; //TEMP
    
    void TestInit()
    {
        GLuint vertexArrayID;
        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);
    
        glm::vec3 m_verts[] {
            {-0.5f, -0.5f, 0.0f},
            { 0.5f, -0.5f, 0.0f},
            { 0.0f,  0.5f, 0.0f},
        };
        //int m_indicies[]{ 0, 1, 2 };
    
        //Generate 1 buffer, put the resulting identifier in m_vertexbuffer
        glGenBuffers(1, &m_vertexbuffer);
        //The following commands will talk about our 'm_vertexbuffer' buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
        //Give our vertices to OpenGL.
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_verts), m_verts, GL_STATIC_DRAW);
    }
    
    void TestTriangle()
    {
        //1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
        glVertexAttribPointer(
            0,                  //attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  //size
            GL_FLOAT,           //type
            GL_FALSE,           //normalized?
            0,                  //stride
            (void*)0            //array buffer offset
        );
        //Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); //Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);
    }
    
    void TestProcessInputTick(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
    
    void TestProcessInputEvent(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/)
    {
        if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        {
            std::cout << "1\n";
        }
    }
    
    void framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
    
    Engine::Engine(const char* windowName, glm::uvec2 size)
    {    
        GLFWwindow* window;

        //Initialize the library GLFW
        if (!glfwInit())
        {
            std::cout << "Failed to Init GLFW\n";
            return;
        }
    
        //Create a windowed mode window and its OpenGL context
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        window = glfwCreateWindow(size.x, size.y, windowName, NULL, NULL);
        if (!window)
        {
            std::cout << "Failed to Create Window\n";
            glfwTerminate();
            return;
        }
        //Make the window's context current
        glfwMakeContextCurrent(window);
    
        //Init Glad
        if (!gladLoadGL()) {
            std::cout << "Failed to initialize OpenGL context\n";
            return;
        }
        if (!GLAD_GL_VERSION_3_0)
        {
            std::cout << "Open GL version too low!\n";
            return;
        }
    
        //setup current window size
        glViewport(0, 0, size.x, size.y);
        //Listen to resize events
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
        //Listen to input events
        glfwSetKeyCallback(window, TestProcessInputEvent);
    
        //Init buffers
        //TestInit();
        Rendering::Renderer::Submit(
            MeshBasicSolidColor{ 
                std::vector<Vertex>{
                    {-0.5f, -0.5f, 0.0f},
                    { 0.5f, -0.5f, 0.0f},
                    { 0.0f,  0.5f, 0.0f},
                } /*inds*/ }
        );

        Rendering::Renderer::Submit(
            MeshBasicSolidColor{
                std::vector<Vertex>{
                    {-0.75f, -0.75f, 0.0f},
                    { -0.25f, -0.75f, 0.0f},
                    { -0.5f,  -0.5f, 0.0f},
                } /*inds*/ }
        );

        // Create and compile our GLSL program from the shaders
        GLuint programID = Rendering::LoadShaderLiterals(
            Rendering::ShaderLiterals::currentVertexShader,
            Rendering::ShaderLiterals::currentFragmentShader
        );
        //GLuint programID = Rendering::LoadShaders(
        //    R"(D:\Callum\Desktop\PooleEngineSandbox\build\Sandbox\Debug\basic_vertex.vertexshader)", 
        //    R"(D:\Callum\Desktop\PooleEngineSandbox\build\Sandbox\Debug\basic_fragment.fragmentshader)"
        //    "basic_vertex.vertexshader",
        //    "basic_fragment.fragmentshader"
        //);
    
        //Loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            //Get input tick
            TestProcessInputTick(window);
    
            //Draw BG
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //Use Shader
            glUseProgram(programID);
    
            //Draw Mesh
            //TestTriangle();
            Rendering::Renderer::RenderAll();
    
            //Swap front and back buffers
            glfwSwapBuffers(window);
    
            //Poll for and process events
            glfwPollEvents();
        }
    
        glfwTerminate();
    }
}