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
        m_DebugText->SetTextView("0 FPS");
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
        float fps = timeData.GetVolatileFPS();
        fps = std::floor(fps);

        //Float to String, remove trailing zeros. TODO make it less shit. printf or something!
        std::string fpsStr = std::to_string(fps);
        fpsStr.erase(fpsStr.find_last_not_of('0') + 1, std::string::npos);
        fpsStr.erase(fpsStr.find_last_not_of('.') + 1, std::string::npos);
        fpsStr += " FPS";

        m_DebugText->SetText(fpsStr);
    }
}