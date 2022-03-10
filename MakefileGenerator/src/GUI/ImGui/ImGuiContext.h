#pragma once
#include <functional>

namespace IGA {

	template <class T>
	class ImGuiContext
	{
	private:
		const char* m_Name;
		const std::function<void(T)> m_Func;
		const T m_PassedArg;
	public:
		ImGuiContext(const char* name, const std::function<void(T)>& func, const T& passFuncArg)
			: m_Name(name), m_Func(func), m_PassedArg(passFuncArg) {}

		void OnResize()
		{
			m_Func(m_PassedArg);
		}
	};
}