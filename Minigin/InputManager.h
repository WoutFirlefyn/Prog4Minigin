#pragma once
#include "Windows.h"
//#include <SDL_scancode.h>
#include <memory>
#include <vector>
#include "Singleton.h"
#include "Controller.h"
#include "Input.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager();
		bool ProcessInput();
		void BindCommand(std::unique_ptr<Command>&& pCommand, unsigned int button, InputType triggerType, uint8_t controllerIdx = -1);
		void AddController(int amount = 1);
	private:
		std::vector<InputAction> m_vKeyboardInputAction{};
		std::vector<std::unique_ptr<Controller>> m_vControllers{};
		static constexpr int m_MaxControllers = 4;
		//std::vector<InputAction> m_vControllerInputAction{};
	};

}
