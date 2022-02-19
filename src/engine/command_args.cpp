#include "command_args.h"

#include "engine.h"

namespace Poole
{
    void CommandArgs::Apply(Engine& engine)
    {
	    const std::vector<std::string_view>& args = engine.m_RunData.m_CommandArgs;

        //The 0th arg will always be the path which is why it's skipped here.
        for (size_t i = 1; i < args.size(); i++)
        {
            std::string_view view = args[i];

            for (const auto& pair : s_Assignments.m_Data)
            {
                if (view.starts_with(pair.first))
                {
                    const std::string_view remainder = view.substr(pair.first.size());
                    (this->*pair.second)(engine, remainder);
                    break;
                }
            }
        }
    }

    void CommandArgs::AssignSize(Engine& engine, std::string_view assignee)
    {
        const size_t comma = assignee.find(',');
        if (comma != std::string_view::npos)
        {
            const std::string_view left = assignee.substr(0, comma);
            const std::string_view right = assignee.substr(comma + 1);
            if (left.size() > 0 && right.size() > 0)
            {
                u32 width, height;
                auto resultW = std::from_chars(left.data(), left.data() + left.size(), width);
                auto resultH = std::from_chars(right.data(), right.data() + right.size(), height);
                if (resultW.ec == std::errc::invalid_argument) {
                    LOG_ERROR("{} size= Width in valid format - ", errorMsg, left);
                    return;
                }
                if (resultH.ec == std::errc::invalid_argument) {
                    LOG_ERROR("{} size= Height in valid format - ", errorMsg, right);
                    return;
                }
                engine.m_RunData.m_Size.x = width;
                engine.m_RunData.m_Size.y = height;
            }
            else
            {
                LOG_ERROR("{} size= didn't provide a width and height", errorMsg);
            }
        }
        else
        {
            LOG_ERROR("{} size= didn't include a comma", errorMsg);
        }
    }
    void CommandArgs::AssignName(Engine& engine, std::string_view assignee)
    {
        engine.m_RunData.m_WindowName = assignee.data(); //Okay since both are valid for lifetime of program
    }
}

