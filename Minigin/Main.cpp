#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <windows.h>
#include <Xinput.h>
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Command.h"
#include "Controller.h"
#include "TextObject.h"
#include "Scene.h"
#include "GraphicsComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RotateComponent.h"
#include "GraphComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto& input = dae::InputManager::GetInstance();

	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::GraphicsComponent>(go.get(), "background.tga");
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::GraphicsComponent>(go.get(), "logo.tga");
	go->SetPosition(216, 180);
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto pTitle = std::make_unique<dae::GameObject>();
	pTitle->AddComponent<dae::TextComponent>(pTitle.get(), "Programming 4 Assignment", font);
	pTitle->SetPosition(80, 20);

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextComponent>(go.get(), "0 FPS", font);
	go->AddComponent<dae::FPSComponent>(go.get());
	go->SetPosition(0, 50);
	go->SetParent(pTitle.get());

	scene.Add(std::move(pTitle));
	scene.Add(std::move(go));

	auto qbert1 = std::make_unique<dae::GameObject>();
	qbert1->AddComponent<dae::GraphicsComponent>(qbert1.get(), "Qbert-P1.tga");
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert1.get(), glm::vec3{0.f, -50.f, 0.f}), SDL_SCANCODE_W, InputType::Down);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert1.get(), glm::vec3{ -50.f, 0.f, 0.f }), SDL_SCANCODE_A, InputType::Down);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert1.get(), glm::vec3{ 0.f, 50.f, 0.f }), SDL_SCANCODE_S, InputType::Down);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert1.get(), glm::vec3{ 50.f, 0.f, 0.f }), SDL_SCANCODE_D, InputType::Down);
	qbert1->SetPosition(100, 400);

	auto qbert2 = std::make_unique<dae::GameObject>();
	qbert2->AddComponent<dae::GraphicsComponent>(qbert2.get(), "Qbert-P2.tga");
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert2.get(), glm::vec3{ 0.f, -100.f, 0.f }), XINPUT_GAMEPAD_DPAD_UP, InputType::Down, false);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert2.get(), glm::vec3{ -100.f, 0.f, 0.f }), XINPUT_GAMEPAD_DPAD_LEFT, InputType::Down, false);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert2.get(), glm::vec3{ 0.f, 100.f, 0.f }), XINPUT_GAMEPAD_DPAD_DOWN, InputType::Down, false);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert2.get(), glm::vec3{ 100.f, 0.f, 0.f }), XINPUT_GAMEPAD_DPAD_RIGHT, InputType::Down, false);
	qbert2->SetPosition(150, 400);

	scene.Add(std::move(qbert1));
	scene.Add(std::move(qbert2));

	// Trash the cache 
	//go = std::make_unique<dae::GameObject>();
	//go->AddComponent<dae::GraphComponent>(go.get());
	//scene.Add(std::move(go));
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}