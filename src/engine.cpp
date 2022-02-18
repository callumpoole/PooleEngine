#include "engine.h"
#include <iostream>
#include <memory>
#include <chrono>
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
        EngineTime::s_LaunchSinceEpochNS = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        
        //Initialize the library GLFW
        if (!glfwInit())
        {
            LOG_FATAL("Failed to Init GLFW");
        }

        GLFWwindow* window = Window::Init(m_RunData.windowName, m_RunData.size);
        if (!window)
        {
            return;
        }

        //Init Glad
        if (!gladLoadGL())
        {
            LOG_FATAL("Failed to initialize OpenGL context");
        }
        if (!GLAD_GL_VERSION_3_0)
        {
            LOG_FATAL("Open GL version too low!");
        }

        //Init Engine Systems
        Window::SetupPostGlad(window);
        Input::Init(window);
        Rendering::Renderer::Init();

        //Init Client Code
        BeginApp();

        //Init time stuff (after client code)
        EngineTime::s_FirstTickSinceEpochNS = std::chrono::high_resolution_clock::now().time_since_epoch().count();

        auto previousFrameTime = std::chrono::high_resolution_clock::now();

        //Loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            //Handle Time Stuff
            HandleTimeParams(previousFrameTime.time_since_epoch().count());
            previousFrameTime = std::chrono::high_resolution_clock::now();

            Input::Tick(window);

            Rendering::Renderer::BeginScene();

            UpdateApp(EngineTime::s_DeltaTime);

            Rendering::Renderer::RenderScene();
            Rendering::Renderer::EndScene(window);

            Window::Tick();
        }

        EndApp();
        Window::Close();
    }

    void Engine::HandleTimeParams(long long PreviousFrameTimeSinceEpochNS)
    {
        const long long NowTimeSinceEpochNS = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        const long long DeltaTimeDuration = NowTimeSinceEpochNS - PreviousFrameTimeSinceEpochNS;

        EngineTime::s_FrameNS = DeltaTimeDuration;
        EngineTime::s_FrameMS = EngineTime::s_FrameNS / 1000'000.f;
        EngineTime::s_DeltaTime = EngineTime::s_FrameMS / 1000.f;
        EngineTime::s_FPS = 1 / EngineTime::s_DeltaTime;
        EngineTime::s_TickCount++;

        const f32 PrevSecondsSinceLaunch = EngineTime::s_SecondsSinceLaunch;
        EngineTime::s_SecondsSinceLaunch    = (NowTimeSinceEpochNS - EngineTime::s_LaunchSinceEpochNS)    / 1000'000'000.f;
        EngineTime::s_SecondsSinceFirstTick = (NowTimeSinceEpochNS - EngineTime::s_FirstTickSinceEpochNS) / 1000'000'000.f;

        EngineTime::s_AccDeltaTimeThisSecond += EngineTime::s_DeltaTime;
        EngineTime::s_AccTicksThisSecond++;

        //If a new second occoured (not an exact science if multiple seconds are skipped, but it's only an approx)
        if (std::floor(PrevSecondsSinceLaunch) != std::floor(EngineTime::s_SecondsSinceLaunch))
        {
            EngineTime::s_AvgDeltaTime = EngineTime::s_AccDeltaTimeThisSecond / EngineTime::s_AccTicksThisSecond;
            EngineTime::s_AvgFPS = 1 / EngineTime::s_AvgDeltaTime;
            EngineTime::s_AccDeltaTimeThisSecond = 0.f;
            EngineTime::s_AccTicksThisSecond = 0;

            Window::SetWindowTitle((Window::GetCurrentTitle() + std::format(" - AvgFPS: {:5.1f}, AvgDTime: {:6.5f}, Open: {:.0f}",
                EngineTime::s_AvgFPS, EngineTime::s_AvgDeltaTime, EngineTime::s_SecondsSinceLaunch)).c_str());
        }

        //LOG("MS: {:7.3f}, DTime: {:8.5f}, FPS: {:7.1f}, AvgDTime: {:8.5f}, AvgFPS: {:7.1f}, Open: {:.2f}",
        //    EngineTime::s_FrameMS, EngineTime::s_DeltaTime, EngineTime::s_FPS, EngineTime::s_AvgDeltaTime, EngineTime::s_AvgFPS, EngineTime::s_SecondsSinceLaunch);
    }
}