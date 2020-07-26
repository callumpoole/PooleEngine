#pragma once

extern Poole::Engine* Poole::CreateApplication();

int main(int /*argc*/, int* /*argv*/[])
{
	auto app = Poole::CreateApplication();
	app->Run();
	delete app;
}