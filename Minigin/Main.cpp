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
//#define Steam
#pragma warning (pop)

#include <windows.h>
#include <Xinput.h>
#include <iostream>
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Input.h"
#include "Command.h"
#include "Controller.h"
#include "TextObject.h"
#include "Scene.h"
#include "Observer.h"
#include "GraphicsComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RotateComponent.h"
#include "GraphComponent.h"
#include "QbertComponent.h"
#include "ScoreComponent.h"
#include "LivesComponent.h"
#include "AchievementComponent.h"
#include "TileComponent.h"
#include "TileSpritesheetComponent.h"
#include "SpritesheetComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto& input = dae::InputManager::GetInstance();

	// Background
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::GraphicsComponent>("background.tga");
	scene.Add(std::move(go));

	// Logo
	//go = std::make_unique<dae::GameObject>();
	//go->AddComponent<dae::GraphicsComponent>("logo.tga");
	//go->SetPosition(216, 180);
	//scene.Add(std::move(go));

	// Title
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto pTitle = std::make_unique<dae::GameObject>();
	//pTitle->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	//pTitle->SetPosition(80, 20);

	// FPS
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextComponent>("0 FPS", font);
	go->AddComponent<dae::FPSComponent>();
	go->SetPosition(10, 10);
	//go->SetParent(pTitle.get());

	//scene.Add(std::move(pTitle));
	scene.Add(std::move(go));


	// Qbert1
	auto qbert1 = std::make_unique<dae::GameObject>();
	qbert1->AddComponent<dae::GraphicsComponent>("Qbert-P1.tga");
	qbert1->AddComponent<dae::QbertComponent>();
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert1.get(), glm::vec3{0.f, -50.f, 0.f}, dae::MovementDirection::Up), SDL_SCANCODE_W, dae::InputType::Pressed);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert1.get(), glm::vec3{ -50.f, 0.f, 0.f }, dae::MovementDirection::Left), SDL_SCANCODE_A, dae::InputType::Pressed);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert1.get(), glm::vec3{ 0.f, 50.f, 0.f }, dae::MovementDirection::Down), SDL_SCANCODE_S, dae::InputType::Pressed);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert1.get(), glm::vec3{ 50.f, 0.f, 0.f }, dae::MovementDirection::Right), SDL_SCANCODE_D, dae::InputType::Pressed);
	input.BindCommand(std::make_unique<dae::KillCommand>(qbert1.get()), SDL_SCANCODE_C, dae::InputType::Pressed);
	input.BindCommand(std::make_unique<dae::ScoreCommand>(qbert1.get(), dae::ScoreType::TileCovered), SDL_SCANCODE_Z, dae::InputType::Pressed);
	input.BindCommand(std::make_unique<dae::ScoreCommand>(qbert1.get(), dae::ScoreType::LevelCleared), SDL_SCANCODE_X, dae::InputType::Pressed);
	qbert1->SetPosition(100, 400);

	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);

	// Control Explain
	auto text1 = std::make_unique<dae::GameObject>();
	text1->AddComponent<dae::TextComponent>("Use WASD to move orange Qbert, C to lose a life, Z to 'cover a tile' and X to 'win'", font);
	text1->SetPosition(10, 60);

	auto text2 = std::make_unique<dae::GameObject>();
	text2->AddComponent<dae::TextComponent>("Use D-Pad to move yellow Qbert, X to lose a life, A to 'cover a tile' and B to 'win'", font);
	text2->SetPosition(0, 25);
	text2->SetParent(text1.get());

	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	// Lives 1
	auto livesDisplay1 = std::make_unique<dae::GameObject>();
	livesDisplay1->AddComponent<dae::TextComponent>(font);
	livesDisplay1->AddComponent<dae::LivesComponent>(qbert1->GetComponent<dae::QbertComponent>());
	livesDisplay1->SetPosition(0, 30);
	livesDisplay1->SetParent(text2.get());

	// Score 1
	auto scoreDisplay1 = std::make_unique<dae::GameObject>();
	scoreDisplay1->AddComponent<dae::TextComponent>(font);
	scoreDisplay1->AddComponent<dae::ScoreComponent>(qbert1->GetComponent<dae::QbertComponent>());
	scoreDisplay1->SetPosition(0, 30);
	scoreDisplay1->SetParent(livesDisplay1.get());



	// Qbert2
	input.AddController();
	auto qbert2 = std::make_unique<dae::GameObject>();
	qbert2->AddComponent<dae::GraphicsComponent>("Qbert-P2.tga");
	qbert2->AddComponent<dae::QbertComponent>();
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert2.get(), glm::vec3{ 0.f, -100.f, 0.f }), XINPUT_GAMEPAD_DPAD_UP, dae::InputType::Down, 0);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert2.get(), glm::vec3{ -100.f, 0.f, 0.f }), XINPUT_GAMEPAD_DPAD_LEFT, dae::InputType::Down, 0);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert2.get(), glm::vec3{ 0.f, 100.f, 0.f }), XINPUT_GAMEPAD_DPAD_DOWN, dae::InputType::Down, 0);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert2.get(), glm::vec3{ 100.f, 0.f, 0.f }), XINPUT_GAMEPAD_DPAD_RIGHT, dae::InputType::Down, 0);
	input.BindCommand(std::make_unique<dae::KillCommand>(qbert2.get()), XINPUT_GAMEPAD_X, dae::InputType::Pressed, 0);
	input.BindCommand(std::make_unique<dae::ScoreCommand>(qbert2.get(), dae::ScoreType::TileCovered), XINPUT_GAMEPAD_A, dae::InputType::Pressed, 0);
	input.BindCommand(std::make_unique<dae::ScoreCommand>(qbert2.get(), dae::ScoreType::LevelCleared), XINPUT_GAMEPAD_B, dae::InputType::Pressed, 0);
	qbert2->SetPosition(150, 400);

	// Lives 2
	auto livesDisplay2 = std::make_unique<dae::GameObject>();
	livesDisplay2->AddComponent<dae::TextComponent>(font);
	livesDisplay2->AddComponent<dae::LivesComponent>(qbert2->GetComponent<dae::QbertComponent>());
	livesDisplay2->SetPosition(0, 30);
	livesDisplay2->SetParent(scoreDisplay1.get());

	// Score 2
	auto scoreDisplay2 = std::make_unique<dae::GameObject>();
	scoreDisplay2->AddComponent<dae::TextComponent>(font);
	scoreDisplay2->AddComponent<dae::ScoreComponent>(qbert2->GetComponent<dae::QbertComponent>());
	scoreDisplay2->SetPosition(0, 30);
	scoreDisplay2->SetParent(livesDisplay2.get());

	// Achievements
	//auto achievements = std::make_unique<dae::GameObject>();
	//achievements->AddComponent<dae::AchievementComponent>(qbert1->GetComponent<dae::QbertComponent>(), qbert2->GetComponent<dae::QbertComponent>());
	//scene.Add(std::move(achievements));

	// Tiles
	std::vector<std::vector<dae::GameObject*>> vTiles{};
	glm::vec3 startPos{ 300, 200, 0 };
	const int levelLength{ 7 };

	for (int i{}; i < levelLength; ++i)
	{
		std::vector<dae::GameObject*> vNewTiles{};
		for (int j{}; j < levelLength - i; ++j)
		{
			auto tile = std::make_unique<dae::GameObject>();
			tile->AddComponent<dae::GraphicsComponent>("Qbert Cubes.png");
			tile->AddComponent<dae::TileSpritesheetComponent>(6, 3);
			tile->AddComponent<dae::TileComponent>(qbert1->GetComponent<dae::QbertComponent>());

			tile->SetPosition(startPos.x + 16.f * (j - i), startPos.y + 24.f * (i + j));
			vNewTiles.emplace_back(scene.Add(std::move(tile)));
		}
		vTiles.emplace_back(vNewTiles);
	}

	vTiles[0][0]->GetComponent<dae::TileComponent>()->MoveQbertHere();

	for (size_t i{}; i < vTiles.size(); ++i)
		for (size_t j{}; j < vTiles[i].size(); ++j)
		{
			auto tileComponent = vTiles[i][j]->GetComponent<dae::TileComponent>();
			tileComponent->SetNeighboringTiles(vTiles, i, j);
		}

	scene.Add(std::move(text1));
	scene.Add(std::move(text2));
	scene.Add(std::move(qbert1));
	scene.Add(std::move(livesDisplay1));
	scene.Add(std::move(scoreDisplay1));
	scene.Add(std::move(qbert2));
	scene.Add(std::move(livesDisplay2));
	scene.Add(std::move(scoreDisplay2));
}

int main(int, char*[]) 
{
#ifdef Steam
	if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed).\n";
		return 1;
	}
	std::cout << "Successfully initialized steam.\n";
#endif

	dae::Minigin engine("../Data/");
	engine.Run(load);

#ifdef Steam
	SteamAPI_Shutdown();
#endif
    return 0;
}