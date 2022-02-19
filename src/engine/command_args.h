#pragma once

#include "core.h"

namespace Poole
{
	class Engine;

	class CommandArgs
	{
	public:
		void Apply(Engine& Engine);
	private:
		static constexpr const char* errorMsg = "CommandLine Arg Incorrect";

		using AssignmentFunction = void(CommandArgs::*)(Engine& Engine, std::string_view assignee);

		void AssignSize(Engine& Engine, std::string_view assignee);
		void AssignName(Engine& Engine, std::string_view assignee);

		static constexpr ConstexprMap<std::string_view, AssignmentFunction, 2> s_Assignments = {{{
			{"size=", &CommandArgs::AssignSize},
			{"name=", &CommandArgs::AssignName},
		}}};
	};
}