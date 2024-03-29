#include "engine.h"
#include <iostream>
#include <memory>
#include <chrono>
#include "rendering/renderer.h"
#include "input/input.h"
#include "window/window.h"
#include "engine/command_args.h"
#include "debug/debug_overlay.h"
#include "physics/collision_2D/collision_manager_2D.h"

namespace Poole 
{
    Engine* Engine::m_Instance = nullptr;

    Engine::Engine(std::vector<std::string_view>&& commandArgs, const char* windowName, uvec2 size)
        : m_RunData({ std::move(commandArgs), windowName, size })
    {
        m_Instance = this;
    }

    Engine::Engine(std::vector<std::string_view>&& commandArgs) 
        : Engine(std::move(commandArgs), "Unamed Window", uvec2(640, 480))
    {
        //See main constructor since it's called via this
    }


    void Engine::Run()
    {
        //C++ locale instead of C locale so commas every 3 numbers
        std::locale::global(std::locale(""));

        m_TimeData.m_LaunchSinceEpochNS = std::chrono::high_resolution_clock::now().time_since_epoch().count();

        CommandArgs().Apply(*this);
        
        //Initialize the library GLFW
        if (!glfwInit())
        {
            LOG_FATAL("Failed to Init GLFW");
        }

        GLFWwindow* window = Window::Init(m_RunData.m_WindowName, m_RunData.m_Size);
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
        m_TimeData.m_FirstTickSinceEpochNS = std::chrono::high_resolution_clock::now().time_since_epoch().count();

        auto previousFrameTime = std::chrono::high_resolution_clock::now();

        Poole::Debug::DebugOverlay DebugOverlay;

        //Loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            //Handle Time Stuff
            HandleTimeParams(previousFrameTime.time_since_epoch().count());
            previousFrameTime = std::chrono::high_resolution_clock::now();

            Input::Tick(window);

            Rendering::Renderer::BeginScene();

            ColliderManager2D::BeginScene();

            UpdateApp(m_TimeData.m_DeltaTime);

            DebugOverlay.Update(*this);

            ColliderManager2D::EndScene();

            Rendering::Renderer::EndScene(window);

            Window::Tick();
        }

        Rendering::Renderer::Shutdown();
        EndApp();
        Window::Close();
    }

    void Engine::HandleTimeParams(const i64 PreviousFrameTimeSinceEpochNS)
    {
        const i64 NowTimeSinceEpochNS = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        const i64 DeltaTimeDuration = NowTimeSinceEpochNS - PreviousFrameTimeSinceEpochNS;

        m_TimeData.m_FrameNS = DeltaTimeDuration;
        m_TimeData.m_FrameMS = m_TimeData.m_FrameNS / 1000'000.f;
        m_TimeData.m_DeltaTime = m_TimeData.m_FrameMS / 1000.f;
        m_TimeData.m_FPS = 1 / m_TimeData.m_DeltaTime;
        m_TimeData.m_TickCount++;

        const f32 PrevSecondsSinceLaunch = m_TimeData.m_SecondsSinceLaunch;
        m_TimeData.m_SecondsSinceLaunch    = (NowTimeSinceEpochNS - m_TimeData.m_LaunchSinceEpochNS)    / 1000'000'000.f;
        m_TimeData.m_SecondsSinceFirstTick = (NowTimeSinceEpochNS - m_TimeData.m_FirstTickSinceEpochNS) / 1000'000'000.f;

        m_TimeData.m_AccDeltaTimeThisSecond += m_TimeData.m_DeltaTime;
        m_TimeData.m_AccTicksThisSecond++;

        //If a new second occoured (not an exact science if multiple seconds are skipped, but it's only an approx)
        if (std::floor(PrevSecondsSinceLaunch) != std::floor(m_TimeData.m_SecondsSinceLaunch))
        {
            m_TimeData.m_AvgDeltaTime = m_TimeData.m_AccDeltaTimeThisSecond / m_TimeData.m_AccTicksThisSecond;
            m_TimeData.m_AvgFPS = 1 / m_TimeData.m_AvgDeltaTime;
            m_TimeData.m_AccDeltaTimeThisSecond = 0.f;
            m_TimeData.m_AccTicksThisSecond = 0;

            Window::SetWindowTitle((Window::GetCurrentTitle() + std::format(" - AvgFPS: {:5.1f}, AvgDTime: {:6.5f}s, Open: {:.0f}s",
                m_TimeData.m_AvgFPS, m_TimeData.m_AvgDeltaTime, m_TimeData.m_SecondsSinceLaunch)).c_str());
        }

        //LOG("MS: {:7.3f}, DTime: {:8.5f}, FPS: {:7.1f}, AvgDTime: {:8.5f}, AvgFPS: {:7.1f}, Open: {:.2f}",
        //    m_TimeData.s_FrameMS, m_TimeData.s_DeltaTime, m_TimeData.s_FPS, m_TimeData.s_AvgDeltaTime, m_TimeData.s_AvgFPS, m_TimeData.s_SecondsSinceLaunch);
    }
}