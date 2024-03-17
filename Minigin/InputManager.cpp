#include <SDL.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include "InputManager.h"
#include "Controller.h"
#include "Command.h"

bool dae::InputManager::ProcessInput()
{
	const Uint8* pKeyboardState = SDL_GetKeyboardState(nullptr);
	
	SDL_Event e;
	
	while (SDL_PollEvent(&e)) 
	{
		if (e.type == SDL_QUIT) 
			return false;
		if (e.type == SDL_KEYDOWN) 
		{
			for (const auto& inputAction : m_vKeyboardInputAction)
			{
				if (inputAction.InputType == InputType::Pressed && pKeyboardState[inputAction.Button])
					inputAction.pCommand->Execute();
			}
		}
		if (e.type == SDL_KEYUP)
		{
			for (const auto& inputAction : m_vKeyboardInputAction)
			{
				if (inputAction.InputType == InputType::Released && pKeyboardState[inputAction.Button])
					inputAction.pCommand->Execute();
			}
		}
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	for (const auto& inputAction : m_vKeyboardInputAction)
	{
		if (inputAction.InputType == InputType::Down && pKeyboardState[inputAction.Button])
			inputAction.pCommand->Execute();
	}

	auto& controller = Controller::GetInstance();
	controller.ProcessInput();

	for (const InputAction& inputAction : m_vControllerInputAction)
	{
		switch (inputAction.InputType)
		{
		case InputType::Down:
			if (controller.IsDown(inputAction.Button))
			{
				inputAction.pCommand->Execute();
			}
			break;
		case InputType::Released:
			if (controller.IsUpThisFrame(inputAction.Button))
			{
				inputAction.pCommand->Execute();
			}
			break;
		case InputType::Pressed:
			if (controller.IsPressedThisFrame(inputAction.Button))
			{
				inputAction.pCommand->Execute();
			}
			break;
		case InputType::Joystick:
			if (controller.IsThumbsNotInDeadZone())
			{
				inputAction.pCommand->Execute();
			}
			break;
		}
	}

	return true;
}

void dae::InputManager::BindCommand(std::unique_ptr<Command>&& pCommand, unsigned int button, InputType triggerType, bool isKeyboardInput)
{
	assert(pCommand);
	if (isKeyboardInput)
		m_vKeyboardInputAction.push_back(dae::InputAction(std::move(pCommand), button, triggerType));
	else
		m_vControllerInputAction.push_back(dae::InputAction(std::move(pCommand), button, triggerType));

}
