#pragma once
#include <memory>
#include "Command.h"
namespace dae
{
	enum class InputType
	{
		Pressed,
		Released,
		Down,
		Joystick
	};
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
}