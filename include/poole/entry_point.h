#pragma once

extern poole::engine* poole::create_application();

int main(int /*argc*/, int* /*argv*/[])
{
	auto app = poole::create_application();
	app->run();
	delete app;
}