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
		InputAction(std::unique_ptr<Command>&& pCommand, unsigned int button, InputType inputType, const std::string& sceneName)
			: pCommand{ std::move(pCommand) }
			, Button{ button }
			, InputType{ inputType }
			, SceneName{ sceneName }
		{
		}

		std::unique_ptr<Command> pCommand;
		unsigned int Button;
		InputType InputType;
		std::string SceneName;
	};
}