#include "Poole.h"
//Testing
#include <iostream>

// This will identify our vertex buffer
GLuint vertexbuffer;

void TestInit()
{
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glm::vec3 verts[] {
        {-0.5f, -0.5f, 0.0f},
        { 0.5f, -0.5f, 0.0f},
        { 0.0f,  0.5f, 0.0f},
    };
    //int indicies[]{ 0, 1, 2 };



    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
}

void TestTriangle()
{
    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);
}

PooleEngine::PooleEngine(const char* windowName, glm::uvec2 size)
{    
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(size.x, size.y, windowName, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    if (!gladLoadGL()) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        exit(1010);
    }

    if (GLAD_GL_VERSION_3_0 == 0) {
        exit(1337);
    }

    TestInit();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        TestTriangle();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}