#pragma once
#include "Windows.h"
#include <SDL_scancode.h>
#include <memory>
#include <vector>
#include "Singleton.h"
#include "Command.h"

enum class InputType
{
	Pressed,
	Released,
	Down,
	Joystick
};

namespace dae
{
	struct InputAction
	{
		InputAction(std::unique_ptr<Command>&& pCommand, unsigned int button, InputType inputType)
			: pCommand{ std::move(pCommand) }
			, Button{ button }
			, InputType{ inputType }
		{
		}

		std::unique_ptr<Command> pCommand;
		unsigned int Button;
		InputType InputType;
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void BindCommand(std::unique_ptr<Command>&& pCommand, unsigned int button, InputType triggerType, bool isKeyboardInput = true);
	private:
		std::vector<InputAction> m_vKeyboardInputAction{};
		std::vector<InputAction> m_vControllerInputAction{};
	};

}
