#include "debug_overlay.h"

#include "poole/rendering/text/text_renderer.h"
#include "poole/rendering/text/text_renderer_factory.h"
#include "poole/engine.h"

namespace Poole::Debug
{
    DebugOverlay::DebugOverlay()
    {
        m_DebugText = Poole::Rendering::TextRendererFactory::MakeRenderText();
        m_DebugText->SetSize(0.05f);
        m_DebugText->SetTextView("0fps");
        m_DebugText->SetColor(Colors::Blue<fcolor4>);
        m_DebugText->SetPosition(fvec3{ -1.25f, 0.9f, 0 });
    }

    DebugOverlay::~DebugOverlay()
    {
        m_DebugText.reset();
    }

    void DebugOverlay::Update(const Poole::Engine& engine)
    {
        const EngineTime& timeData = engine.GetTimeData();
        const float fps = timeData.GetAvgFPS();
        const std::string fpsStr = std::format("{:.0f}fps", fps);

        m_DebugText->SetText(fpsStr);
    }
}