#pragma once

#include <vector>
#include <string_view>

extern Poole::Engine* Poole::CreateApplication(std::vector<std::string_view>&& commandArgs);

int main(const int argc, const char* argv[])
{
	std::vector<std::string_view> commandArgs;
	commandArgs.reserve(argc);
	for (size_t i = 0; i < argc; i++)
	{
		commandArgs.emplace_back(argv[i]);
	}

	Poole::Engine* app = Poole::CreateApplication(std::move(commandArgs));
	app->Run();
	delete app;
}
