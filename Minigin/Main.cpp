#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#pragma warning (push)
#pragma warning (disable: 4996)
#include "steam_api.h"
#pragma warning (pop)

#include <windows.h>
#include <Xinput.h>
#include <iostream>
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
#include "QbertComponent.h"
#include "ScoreComponent.h"
#include "LivesComponent.h"
#include "AchievementComponent.h"
#include "Observer.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto& input = dae::InputManager::GetInstance();

	// Background
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::GraphicsComponent>(go.get(), "background.tga");
	scene.Add(std::move(go));

	// Logo
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::GraphicsComponent>(go.get(), "logo.tga");
	go->SetPosition(216, 180);
	scene.Add(std::move(go));

	// Title
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto pTitle = std::make_unique<dae::GameObject>();
	pTitle->AddComponent<dae::TextComponent>(pTitle.get(), "Programming 4 Assignment", font);
	pTitle->SetPosition(80, 20);

	// FPS
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextComponent>(go.get(), "0 FPS", font);
	go->AddComponent<dae::FPSComponent>(go.get());
	go->SetPosition(0, 50);
	go->SetParent(pTitle.get());

	scene.Add(std::move(pTitle));
	scene.Add(std::move(go));


	// Qbert1
	auto qbert1 = std::make_unique<dae::GameObject>();
	qbert1->AddComponent<dae::GraphicsComponent>(qbert1.get(), "Qbert-P1.tga");
	qbert1->AddComponent<dae::QbertComponent>(qbert1.get());
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert1.get(), glm::vec3{0.f, -50.f, 0.f}), SDL_SCANCODE_W, InputType::Down);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert1.get(), glm::vec3{ -50.f, 0.f, 0.f }), SDL_SCANCODE_A, InputType::Down);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert1.get(), glm::vec3{ 0.f, 50.f, 0.f }), SDL_SCANCODE_S, InputType::Down);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert1.get(), glm::vec3{ 50.f, 0.f, 0.f }), SDL_SCANCODE_D, InputType::Down);
	input.BindCommand(std::make_unique<dae::KillCommand>(qbert1.get()), SDL_SCANCODE_C, InputType::Pressed);
	input.BindCommand(std::make_unique<dae::ScoreCommand>(qbert1.get(), dae::ScoreType::TileCovered), SDL_SCANCODE_Z, InputType::Pressed);
	input.BindCommand(std::make_unique<dae::ScoreCommand>(qbert1.get(), dae::ScoreType::LevelCleared), SDL_SCANCODE_X, InputType::Pressed);
	qbert1->SetPosition(100, 400);

	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);

	// Control Explain
	auto text1 = std::make_unique<dae::GameObject>();
	text1->AddComponent<dae::TextComponent>(text1.get(), "Use WASD to move orange Qbert, C to lose a life, Z to 'cover a tile' and X to 'win'", font);
	text1->SetPosition(20, 120);

	auto text2 = std::make_unique<dae::GameObject>();
	text2->AddComponent<dae::TextComponent>(text2.get(), "Use D-Pad to move yellow Qbert, X to lose a life, A to 'cover a tile' and B to 'win'", font);
	text2->SetPosition(0, 25);
	text2->SetParent(text1.get());

	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	// Lives 1
	auto livesDisplay1 = std::make_unique<dae::GameObject>();
	livesDisplay1->AddComponent<dae::TextComponent>(livesDisplay1.get(), font);
	livesDisplay1->AddComponent<dae::LivesComponent>(livesDisplay1.get(), qbert1->GetComponent<dae::QbertComponent>());
	livesDisplay1->SetPosition(0, 30);
	livesDisplay1->SetParent(text2.get());

	// Score 1
	auto scoreDisplay1 = std::make_unique<dae::GameObject>();
	scoreDisplay1->AddComponent<dae::TextComponent>(scoreDisplay1.get(), font);
	scoreDisplay1->AddComponent<dae::ScoreComponent>(scoreDisplay1.get(), qbert1->GetComponent<dae::QbertComponent>());
	scoreDisplay1->SetPosition(0, 30);
	scoreDisplay1->SetParent(livesDisplay1.get());



	// Qbert2
	auto qbert2 = std::make_unique<dae::GameObject>();
	qbert2->AddComponent<dae::GraphicsComponent>(qbert2.get(), "Qbert-P2.tga");
	qbert2->AddComponent<dae::QbertComponent>(qbert2.get());
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert2.get(), glm::vec3{ 0.f, -100.f, 0.f }), XINPUT_GAMEPAD_DPAD_UP, InputType::Down, false);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert2.get(), glm::vec3{ -100.f, 0.f, 0.f }), XINPUT_GAMEPAD_DPAD_LEFT, InputType::Down, false);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert2.get(), glm::vec3{ 0.f, 100.f, 0.f }), XINPUT_GAMEPAD_DPAD_DOWN, InputType::Down, false);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert2.get(), glm::vec3{ 100.f, 0.f, 0.f }), XINPUT_GAMEPAD_DPAD_RIGHT, InputType::Down, false);
	input.BindCommand(std::make_unique<dae::KillCommand>(qbert2.get()), XINPUT_GAMEPAD_X, InputType::Pressed, false);
	input.BindCommand(std::make_unique<dae::ScoreCommand>(qbert2.get(), dae::ScoreType::TileCovered), XINPUT_GAMEPAD_A, InputType::Pressed, false);
	input.BindCommand(std::make_unique<dae::ScoreCommand>(qbert2.get(), dae::ScoreType::LevelCleared), XINPUT_GAMEPAD_B, InputType::Pressed, false);
	qbert2->SetPosition(150, 400);

	// Lives 2
	auto livesDisplay2 = std::make_unique<dae::GameObject>();
	livesDisplay2->AddComponent<dae::TextComponent>(livesDisplay2.get(), font);
	livesDisplay2->AddComponent<dae::LivesComponent>(livesDisplay2.get(), qbert2->GetComponent<dae::QbertComponent>());
	livesDisplay2->SetPosition(0, 30);
	livesDisplay2->SetParent(scoreDisplay1.get());

	// Score 2
	auto scoreDisplay2 = std::make_unique<dae::GameObject>();
	scoreDisplay2->AddComponent<dae::TextComponent>(scoreDisplay2.get(), font);
	scoreDisplay2->AddComponent<dae::ScoreComponent>(scoreDisplay2.get(), qbert2->GetComponent<dae::QbertComponent>());
	scoreDisplay2->SetPosition(0, 30);
	scoreDisplay2->SetParent(livesDisplay2.get());

	// Achievements
	auto achievements = std::make_unique<dae::GameObject>();
	achievements->AddComponent<dae::AchievementComponent>(achievements.get(), qbert1->GetComponent<dae::QbertComponent>());

	scene.Add(std::move(text1));
	scene.Add(std::move(text2));
	scene.Add(std::move(qbert1));
	scene.Add(std::move(livesDisplay1));
	scene.Add(std::move(scoreDisplay1));
	scene.Add(std::move(qbert2));
	scene.Add(std::move(livesDisplay2));
	scene.Add(std::move(scoreDisplay2));
	scene.Add(std::move(achievements));
}

int main(int, char*[]) 
{
	if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
		return 1;
	}
	else
		std::cout << "Successfully initialized steam." << std::endl;

	dae::Minigin engine("../Data/");
	engine.Run(load);

	SteamAPI_Shutdown();
    return 0;
}