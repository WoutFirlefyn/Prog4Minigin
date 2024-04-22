#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

//#include <windows.h>
//#include <Xinput.h>
#include <iostream>
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Input.h"
#include "CommandImplementation.h"
#include "Controller.h"
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
#include "TileComponent.h"
#include "SpritesheetComponent.h"
#include "QbertCurseComponent.h"
#include "LevelManagerComponent.h"
#include "ServiceLocator.h"
#include "SDLSoundSystem.h"

#ifdef STEAM
#pragma warning (push)
#pragma warning (disable: 4996)
#include "steam_api.h"
#pragma warning (pop)
#endif // Steam

void load()
{
#if NDEBUG
	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<SDLSoundSystem>());
#else
	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::LoggingSoundSystem>(std::make_unique<SDLSoundSystem>()));
#endif

	auto& scene = dae::SceneManager::GetInstance().CreateScene("Qbert");
	auto& input = dae::InputManager::GetInstance();

	// Background
	auto backGround = std::make_unique<dae::GameObject>();
	backGround->AddComponent<dae::GraphicsComponent>("background.tga");
	scene.Add(std::move(backGround));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	// FPS
	auto fps = std::make_unique<dae::GameObject>();
	fps->AddComponent<dae::GraphicsComponent>();
	fps->AddComponent<dae::TextComponent>(font);
	fps->AddComponent<FPSComponent>();
	fps->SetPosition(10, 10);

	// Qbert1
	auto qbert1 = std::make_unique<dae::GameObject>();
	qbert1->AddComponent<dae::GraphicsComponent>("Qbert P1 Spritesheet.png");
	qbert1->AddComponent<QbertComponent>();
	qbert1->AddComponent<dae::SpritesheetComponent>(4, 1);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementDirection::Up), SDL_SCANCODE_W, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementDirection::Left), SDL_SCANCODE_A, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementDirection::Down), SDL_SCANCODE_S, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementDirection::Right), SDL_SCANCODE_D, dae::InputType::Down);
	qbert1->SetPosition(308, 193);

	// Tiles
	auto levelManager = std::make_unique<dae::GameObject>();
	levelManager->AddComponent<LevelManagerComponent>(scene);

	// Qbert curse
	auto qbertCurse = std::make_unique<dae::GameObject>();
	qbertCurse->AddComponent<dae::GraphicsComponent>("Qbert Curses.png", false);
	qbertCurse->AddComponent<QbertCurseComponent>(levelManager->GetComponent<LevelManagerComponent>()->CharacterFell.get());
	qbertCurse->SetParent(qbert1.get());
	qbertCurse->SetPosition(-18, -30);

	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	// Lives 1
	std::vector<std::unique_ptr<dae::GameObject>> vHearts{};

	for (size_t i{}; i < 3; ++i)
	{
		vHearts.push_back(std::make_unique<dae::GameObject>());
		vHearts[i]->AddComponent<dae::GraphicsComponent>("Heart.png");
		vHearts[i]->AddComponent<LivesComponent>();
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
	scoreDisplay->AddComponent<ScoreComponent>();
	scoreDisplay->SetPosition(10, 50);


	// Observers
	auto pLevelManagerComponent = levelManager->GetComponent<LevelManagerComponent>();
	auto pQbertComponent1 = qbert1->GetComponent<QbertComponent>();

	pLevelManagerComponent->AddObserver(pQbertComponent1->MoveStateChanged.get());
	pQbertComponent1->AddObserver(pLevelManagerComponent->TileChanged.get(), pLevelManagerComponent->CharacterFell.get());
	for (auto& heart : vHearts)
		heart->GetComponent<LivesComponent>()->AddObserver(pQbertComponent1->PlayerDied.get());
	scoreDisplay->GetComponent<ScoreComponent>()->AddObserver(pLevelManagerComponent->TileChanged.get());


	// Add to scene
	auto levelManagerTemp = scene.Add(std::move(levelManager));
	scene.Add(std::move(fps));
	scene.Add(std::move(scoreDisplay));
	for (auto& heart : vHearts)
		scene.Add(std::move(heart));
	scene.Add(std::move(qbertCurse));
	auto qbertTemp = scene.Add(std::move(qbert1));

	// Temp way of adding qbert to the level
	levelManagerTemp->GetComponent<LevelManagerComponent>()->SpawnQbert(qbertTemp);

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