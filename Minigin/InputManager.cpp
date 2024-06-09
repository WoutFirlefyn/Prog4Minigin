#include <SDL.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include "InputManager.h"
#include "Scene.h"
#include "Controller.h"
#include "Command.h"

dae::InputManager::InputManager() = default;
dae::InputManager::~InputManager() = default;

bool dae::InputManager::ProcessInput()
{
	const Uint8* pKeyboardState = SDL_GetKeyboardState(nullptr);
	
	SDL_Event e;
	
	while (SDL_PollEvent(&e)) 
	{
		if (e.type == SDL_QUIT) 
			return false;
		if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
		{
			for (const auto& inputAction : m_vKeyboardInputAction)
			{
				if (e.type == SDL_KEYDOWN && inputAction.InputType == InputType::Pressed && pKeyboardState[inputAction.Button])
					inputAction.pCommand->Execute();
				if (e.type == SDL_KEYUP && inputAction.InputType == InputType::Released && static_cast<unsigned int>(e.key.keysym.scancode) == inputAction.Button)
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

	for (auto& controller : m_vControllers)
		controller->ProcessInput();

	return true;
}

void dae::InputManager::BindCommand(const std::shared_ptr<Command>& pCommand, unsigned int button, InputType triggerType)
{
	assert(pCommand);
	m_vKeyboardInputAction.push_back(dae::InputAction(pCommand, button, triggerType));
}

void dae::InputManager::BindCommand(const std::shared_ptr<Command>& pCommand, ControllerButton button, InputType triggerType, uint8_t controllerIdx)
{
	assert(pCommand);

	if (controllerIdx >= 0 && controllerIdx < 4)
	{
		if (controllerIdx >= m_vControllers.size())
			AddController(static_cast<int>(controllerIdx) - static_cast<int>(m_vControllers.size()) + 1);
		m_vControllers[controllerIdx]->BindCommand(pCommand, button, triggerType);
	}
	else
		assert(false && "Trying to bind a command to a controller that doesn't exist");
}

void dae::InputManager::ClearInputActions()
{
	m_vKeyboardInputAction.clear();
	for (auto& pController : m_vControllers)
		pController->ClearInputActions();
	m_vControllers.clear();
}

void dae::InputManager::AddController(int amount)
{
	if (m_vControllers.size() >= m_MaxControllers)
	{
		assert(false && "Max amount of controllers reached");
		return;
	}

	amount = (std::min)(m_MaxControllers - static_cast<int>(m_vControllers.size()), amount);

	for (int i{}; i < amount; ++i)
		m_vControllers.push_back(std::make_unique<Controller>(static_cast<uint8_t>(m_vControllers.size())));
}
