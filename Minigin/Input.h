#pragma once
#include <memory>
#include <string>
#include "Command.h"
#include "Scene.h"
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
		InputAction(const std::shared_ptr<Command>& pCommand, unsigned int button, InputType inputType)
			: pCommand{ pCommand }
			, Button{ button }
			, InputType{ inputType }
		{
		}

		std::shared_ptr<Command> pCommand;
		unsigned int Button;
		InputType InputType;
	};
}