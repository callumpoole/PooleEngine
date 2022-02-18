#pragma once

#include "core.h"
#include <functional>

struct GLFWwindow;

namespace Poole
{
	class Window
	{
	public:
		[[nodiscard]]
		static GLFWwindow* Init(const char* title, uvec2 size);
		static void SetupPostGlad(GLFWwindow* window);
		static void Tick();
		static void Close();
		static void SetWindowTitle(const char* title);

		static std::vector<std::function<void(int width, int height)>> m_OnResize;

		static std::string GetOriginalTitle() { return m_OriginalTitle; }
		static std::string GetCurrentTitle() { return m_Title; }
		static uvec2 GetWindowSize() { return m_WindowSize; }
		static GLFWwindow* m_WindowInstance;
	private:
		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
		static std::string m_OriginalTitle;
		static std::string m_Title;
		static uvec2 m_WindowSize;
	};
}
