#pragma once
#include "Windows.h"
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
		void BindCommand(const std::shared_ptr<Command>& pCommand, unsigned int button, InputType triggerType);
		void BindCommand(const std::shared_ptr<Command>& pCommand, ControllerButton button, InputType triggerType, uint8_t controllerIdx = 0);
		void ClearInputActions();
	private:
		void AddController(int amount = 1);
		std::vector<InputAction> m_vKeyboardInputAction{};
		std::vector<std::unique_ptr<Controller>> m_vControllers{};
		static constexpr int m_MaxControllers = 4;
	};
}
