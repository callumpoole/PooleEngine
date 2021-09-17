#pragma once

extern Poole::Engine* Poole::CreateApplication();

int main(int /*argc*/, int* /*argv*/[])
{
	Poole::Engine* app = Poole::CreateApplication();
	app->Run();
	delete app;
}