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
            const std::string_view view = args[i];

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
        if (auto splitResult = Split(assignee, ','))
        {
            std::optional<u32> width = ToPrimitive<u32>(splitResult->first);
            if (!width)
            {
                LOG_ERROR("{} size= Width in valid format - {}", errorMsg, splitResult->first);
                return;
            }
            std::optional<u32> height = ToPrimitive<u32>(splitResult->second);
            if (!height)
            {
                LOG_ERROR("{} size= Height in valid format - {}", errorMsg, splitResult->second);
                return;
            }

            engine.m_RunData.m_Size = { *width, *height };
        }
        else
        {
            LOG_ERROR("{} size= didn't include a comma - {}", errorMsg, assignee);
        }
    }
    void CommandArgs::AssignName(Engine& engine, std::string_view assignee)
    {
        engine.m_RunData.m_WindowName = assignee.data(); //Okay since both are valid for lifetime of program
    }
}
