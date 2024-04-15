#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

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
#include "SpritesheetComponent.h"
#include "QbertCurseComponent.h"

#ifdef STEAM
#pragma warning (push)
#pragma warning (disable: 4996)
#include "steam_api.h"
#pragma warning (pop)
#endif // Steam

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto& input = dae::InputManager::GetInstance();

	// Background
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::GraphicsComponent>("background.tga");
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	// FPS
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::GraphicsComponent>();
	go->AddComponent<dae::TextComponent>(font);
	go->AddComponent<dae::FPSComponent>();
	go->SetPosition(10, 10);
	scene.Add(std::move(go));

	// Qbert1
	auto qbert1 = std::make_unique<dae::GameObject>();
	qbert1->AddComponent<dae::GraphicsComponent>("Qbert P1 Spritesheet.png");
	qbert1->AddComponent<dae::QbertComponent>();
	qbert1->AddComponent<dae::SpritesheetComponent>(4, 1);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert1.get(), dae::MovementDirection::Up), SDL_SCANCODE_W, dae::InputType::Pressed);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert1.get(), dae::MovementDirection::Left), SDL_SCANCODE_A, dae::InputType::Pressed);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert1.get(), dae::MovementDirection::Down), SDL_SCANCODE_S, dae::InputType::Pressed);
	input.BindCommand(std::make_unique<dae::MoveCommand>(qbert1.get(), dae::MovementDirection::Right), SDL_SCANCODE_D, dae::InputType::Pressed);
	qbert1->SetPosition(308, 193);

	auto qbertCurse = std::make_unique<dae::GameObject>();
	qbertCurse->AddComponent<dae::GraphicsComponent>("Qbert Curses.png", false);
	qbertCurse->AddComponent<dae::QbertCurseComponent>(qbert1->GetComponent<dae::QbertComponent>());
	qbertCurse->SetParent(qbert1.get());
	qbertCurse->SetPosition(-18, -30);

	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	// Lives 1
	std::vector<std::unique_ptr<dae::GameObject>> vHearts{};

	for (int i{}; i < 3; ++i)
	{
		vHearts.push_back(std::make_unique<dae::GameObject>());
		vHearts[i]->AddComponent<dae::GraphicsComponent>("Heart.png");
		vHearts[i]->AddComponent<dae::LivesComponent>(qbert1->GetComponent<dae::QbertComponent>());

		if (i != 0)
		{
			vHearts[i]->SetParent(vHearts[i - 1].get());
			vHearts[i]->SetPosition(0, 18);
		}
	}

	vHearts[0]->SetPosition(10, 80);

	// Score 1
	auto scoreDisplay = std::make_unique<dae::GameObject>();
	scoreDisplay->AddComponent<dae::GraphicsComponent>();
	scoreDisplay->AddComponent<dae::TextComponent>(font);
	scoreDisplay->AddComponent<dae::ScoreComponent>(qbert1->GetComponent<dae::QbertComponent>());
	scoreDisplay->SetPosition(10, 50);
	dae::TileComponent::TileChanged->AddObserver(scoreDisplay->GetComponent<dae::ScoreComponent>());

	for (auto& heart : vHearts)
		scene.Add(std::move(heart));

	scene.Add(std::move(scoreDisplay));

	// Qbert2
	//input.AddController();
	//auto qbert2 = std::make_unique<dae::GameObject>();
	//qbert2->AddComponent<dae::GraphicsComponent>("Qbert-P2.tga");
	//qbert2->AddComponent<dae::QbertComponent>();
	//input.BindCommand(std::make_unique<dae::MoveCommand>(qbert2.get(), glm::vec3{ 0.f, -100.f, 0.f }), XINPUT_GAMEPAD_DPAD_UP, dae::InputType::Down, 0);
	//input.BindCommand(std::make_unique<dae::MoveCommand>(qbert2.get(), glm::vec3{ -100.f, 0.f, 0.f }), XINPUT_GAMEPAD_DPAD_LEFT, dae::InputType::Down, 0);
	//input.BindCommand(std::make_unique<dae::MoveCommand>(qbert2.get(), glm::vec3{ 0.f, 100.f, 0.f }), XINPUT_GAMEPAD_DPAD_DOWN, dae::InputType::Down, 0);
	//input.BindCommand(std::make_unique<dae::MoveCommand>(qbert2.get(), glm::vec3{ 100.f, 0.f, 0.f }), XINPUT_GAMEPAD_DPAD_RIGHT, dae::InputType::Down, 0);
	//input.BindCommand(std::make_unique<dae::KillCommand>(qbert2.get()), XINPUT_GAMEPAD_X, dae::InputType::Pressed, 0);
	//input.BindCommand(std::make_unique<dae::ScoreCommand>(qbert2.get(), dae::ScoreType::TileCovered), XINPUT_GAMEPAD_A, dae::InputType::Pressed, 0);
	//input.BindCommand(std::make_unique<dae::ScoreCommand>(qbert2.get(), dae::ScoreType::LevelCleared), XINPUT_GAMEPAD_B, dae::InputType::Pressed, 0);
	//qbert2->SetPosition(150, 400);

	// Lives 2
	//auto livesDisplay2 = std::make_unique<dae::GameObject>();
	//livesDisplay2->AddComponent<dae::TextComponent>(font);
	//livesDisplay2->AddComponent<dae::LivesComponent>(qbert2->GetComponent<dae::QbertComponent>());
	//livesDisplay2->SetPosition(0, 30);
	//livesDisplay2->SetParent(scoreDisplay1.get());

	//// Score 2
	//auto scoreDisplay2 = std::make_unique<dae::GameObject>();
	//scoreDisplay2->AddComponent<dae::TextComponent>(font);
	//scoreDisplay2->AddComponent<dae::ScoreComponent>(qbert2->GetComponent<dae::QbertComponent>());
	//scoreDisplay2->SetPosition(0, 30);
	//scoreDisplay2->SetParent(livesDisplay2.get());

	// Achievements
	//auto achievements = std::make_unique<dae::GameObject>();
	//achievements->AddComponent<dae::AchievementComponent>(qbert1->GetComponent<dae::QbertComponent>(), qbert2->GetComponent<dae::QbertComponent>());
	//scene.Add(std::move(achievements));

	// Tiles
	std::vector<std::vector<dae::GameObject*>> vTiles{};
	glm::vec2 startPos{ 300, 200 };
	const int levelLength{ 7 };

	for (int i{}; i < levelLength; ++i)
	{
		std::vector<dae::GameObject*> vNewTiles{};
		for (int j{}; j < levelLength - i; ++j)
		{
			auto tile = std::make_unique<dae::GameObject>();
			tile->AddComponent<dae::GraphicsComponent>("Qbert Cubes.png");
			tile->AddComponent<dae::SpritesheetComponent>(6, 3);
			tile->AddComponent<dae::TileComponent>(qbert1->GetComponent<dae::QbertComponent>());

			tile->SetPosition(startPos.x + 16.f * (j - i), startPos.y + 24.f * (i + j));
			vNewTiles.emplace_back(scene.Add(std::move(tile)));
		}
		vTiles.emplace_back(vNewTiles);
	}

	for (size_t i{}; i < vTiles.size(); ++i)
		for (size_t j{}; j < vTiles[i].size(); ++j)
			vTiles[i][j]->GetComponent<dae::TileComponent>()->SetNeighboringTiles(vTiles, i, j);

	scene.Add(std::move(qbertCurse));
	scene.Add(std::move(qbert1));
}

int main(int, char*[]) 
{
#ifdef STEAM
	if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed).\n";
		return 1;
	}
	std::cout << "Successfully initialized steam.\n";
#endif // Steam

	dae::Minigin engine("../Data/");
	engine.Run(load);

#ifdef STEAM
	SteamAPI_Shutdown();
#endif // Steam
    return 0;
}