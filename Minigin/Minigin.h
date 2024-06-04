#pragma once
//#define STEAM
#include <string>
#include <functional>
#include <glm/glm.hpp>

namespace dae
{
	class Minigin
	{
	public:
		explicit Minigin();
		~Minigin();
		void Run(const std::function<void()>& load);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

		static const glm::ivec2 m_WindowSize;
	};
}