#include <SDL.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include "InputManager.h"

bool dae::InputManager::ProcessInput()
{
	//ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	//XInputGetState(0, &m_CurrentState);
	const Uint8* pKeyboardState = SDL_GetKeyboardState(nullptr);
	//SDL_Scancode
	
	SDL_Event e;
	
	while (SDL_PollEvent(&e)) 
	{
		if (e.type == SDL_QUIT) 
			return false;
		if (e.type == SDL_KEYDOWN) 
		{
			for (const auto& inputAction : m_vInputActionKeyboard)
			{
				if (inputAction.InputType == InputType::Pressed && pKeyboardState[inputAction.Button])
					inputAction.pCommand->Execute();
			}
		}
		if (e.type == SDL_KEYUP)
		{
			for (const auto& inputAction : m_vInputActionKeyboard)
			{
				if (inputAction.InputType == InputType::Released && pKeyboardState[inputAction.Button])
					inputAction.pCommand->Execute();
			}
		}
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	for (const auto& inputAction : m_vInputActionKeyboard)
	{
		if (inputAction.InputType == InputType::Down && pKeyboardState[inputAction.Button])
			inputAction.pCommand->Execute();
	}

	return true;
}

void dae::InputManager::BindCommand(std::unique_ptr<Command>&& pCommand, SDL_Scancode button, InputType triggerType)
{
	assert(pCommand);
	m_vInputActionKeyboard.push_back(dae::KeyboardInputAction(std::move(pCommand), button, triggerType));
}
