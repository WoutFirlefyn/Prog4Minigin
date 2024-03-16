#pragma once
#include <SDL_scancode.h>
#include <memory>
#include <vector>
#include "Singleton.h"
#include "Command.h"

enum class InputType
{
	Pressed,
	Released,
	Down
};

namespace dae
{
	struct KeyboardInputAction
	{
		KeyboardInputAction(std::unique_ptr<Command>&& pCommand, SDL_Scancode button, InputType inputType)
			: pCommand{ std::move(pCommand) }
			, Button{ button }
			, InputType{ inputType }
		{
		}

		std::unique_ptr<Command> pCommand;
		SDL_Scancode Button;
		InputType InputType;
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void BindCommand(std::unique_ptr<Command>&& pCommand, SDL_Scancode button, InputType triggerType);
	private:
		std::vector<KeyboardInputAction> m_vInputActionKeyboard{};
	};

}
