#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

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
#include "CoilyComponent.h"
#include "ScoreComponent.h"
#include "LivesComponent.h"
#include "TileComponent.h"
#include "SpritesheetComponent.h"
#include "QbertCurseComponent.h"
#include "LevelManagerComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"

#ifdef STEAM
#pragma warning (push)
#pragma warning (disable: 4996)
#include "steam_api.h"
#pragma warning (pop)
#endif // Steam

void load()
{
#ifdef NDEBUG
	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::SDLSoundSystem>());
	dae::ServiceLocator::RegisterResourceManager(std::make_unique<dae::ResourceManager>());
#else
	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::LoggingSoundSystem>(std::make_unique<dae::SDLSoundSystem>()));
	dae::ServiceLocator::RegisterResourceManager(std::make_unique<dae::LoggingResourceManager>(std::make_unique<dae::ResourceManager>()));
#endif

	auto& scene = dae::SceneManager::GetInstance().CreateScene("Qbert");
	auto& input = dae::InputManager::GetInstance();
	auto& soundSystem = dae::ServiceLocator::GetSoundSystem();
	auto& resourceManager = dae::ServiceLocator::GetResourceManager();

#pragma region dae::Sounds
	soundSystem.LoadSound("Change Selection.wav", dae::Sounds::ChangeSelection);
	soundSystem.LoadSound("Clear Disks.wav", dae::Sounds::ClearDisks);
	soundSystem.LoadSound("Coily Egg Jump.wav", dae::Sounds::CoilyEggJump);
	soundSystem.LoadSound("Coily Fall.wav", dae::Sounds::CoilyFall);
	soundSystem.LoadSound("Coily Snake Jump.wav", dae::Sounds::CoilySnakeJump);
	soundSystem.LoadSound("Disk Land.wav", dae::Sounds::DiskLand);
	soundSystem.LoadSound("Disk Lift.wav", dae::Sounds::DiskLift);
	soundSystem.LoadSound("Level Screen Tune.wav", dae::Sounds::LevelScreenTune);
	soundSystem.LoadSound("Other Foes Jump.wav", dae::Sounds::OtherFoesJump);
	soundSystem.LoadSound("QBert Fall.wav", dae::Sounds::QbertFall);
	soundSystem.LoadSound("QBert Hit.wav", dae::Sounds::QbertHit);
	soundSystem.LoadSound("QBert Jump.wav", dae::Sounds::QbertJump);
	soundSystem.LoadSound("Round Complete Tune.wav", dae::Sounds::RoundCompleteTune);
	soundSystem.LoadSound("SlickSam Caught.wav", dae::Sounds::SlickSamCaught);
	soundSystem.LoadSound("Swearing.wav", dae::Sounds::Swearing);
#pragma endregion

	// Background
	auto backGround = std::make_unique<dae::GameObject>();
	backGround->AddComponent<dae::GraphicsComponent>("background.tga");
	scene.Add(std::move(backGround));

	auto font = resourceManager.LoadFont("Minecraft.ttf", 36);

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
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementDirection::Up), dae::ControllerButton::DPAD_UP, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementDirection::Left), dae::ControllerButton::DPAD_LEFT, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementDirection::Down), dae::ControllerButton::DPAD_DOWN, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementDirection::Right), dae::ControllerButton::DPAD_RIGHT, dae::InputType::Down);
	qbert1->SetPosition(309, 193);
	
	// Coily
	auto coily = std::make_unique<dae::GameObject>();
	coily->AddComponent<dae::GraphicsComponent>("Coily Spritesheet.png");
	coily->AddComponent<CoilyComponent>();
	coily->AddComponent<dae::SpritesheetComponent>(5, 2);
	input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementDirection::Up), SDL_SCANCODE_UP, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementDirection::Left), SDL_SCANCODE_LEFT, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementDirection::Down), SDL_SCANCODE_DOWN, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementDirection::Right), SDL_SCANCODE_RIGHT, dae::InputType::Down);
	coily->SetPosition(309, 180);

	// Tiles
	auto levelManager = std::make_unique<dae::GameObject>();
	levelManager->AddComponent<LevelManagerComponent>(scene);

	// Qbert curse
	auto qbertCurse = std::make_unique<dae::GameObject>();
	qbertCurse->AddComponent<dae::GraphicsComponent>("Qbert Curses.png", false);
	qbertCurse->AddComponent<QbertCurseComponent>();
	qbertCurse->SetParent(qbert1.get());
	qbertCurse->SetPosition(-18, -30);

	font = resourceManager.LoadFont("Minecraft.ttf", 20);

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

	font = resourceManager.LoadFont("Minecraft.ttf", 15);
	// Control display
	auto controlDisplay = std::make_unique<dae::GameObject>();
	controlDisplay->AddComponent<dae::GraphicsComponent>();
	controlDisplay->AddComponent<dae::TextComponent>("WASD/DPAD to move. Move, fall off map, jump on disc or complete level for sounds", font);
	controlDisplay->SetPosition(10, 450);


	// Observers
	auto pLevelManagerComponent = levelManager->GetComponent<LevelManagerComponent>();
	auto pQbertComponent1 = qbert1->GetComponent<QbertComponent>();

	pLevelManagerComponent->AddObserver(pQbertComponent1->MoveStateChanged.get());
	pQbertComponent1->AddObserver(pLevelManagerComponent->TileChanged.get(), pLevelManagerComponent->CharacterStartedJumping.get());
	for (auto& heart : vHearts)
		heart->GetComponent<LivesComponent>()->AddObserver(pQbertComponent1->PlayerDied.get());
	scoreDisplay->GetComponent<ScoreComponent>()->AddObserver(pLevelManagerComponent->TileChanged.get());
	qbertCurse->GetComponent<QbertCurseComponent>()->AddObserver(pQbertComponent1->PlayerDied.get());


	// Add to scene
	auto levelManagerTemp = scene.Add(std::move(levelManager));
	scene.Add(std::move(fps));
	scene.Add(std::move(scoreDisplay));
	for (auto& heart : vHearts)
		scene.Add(std::move(heart));
	scene.Add(std::move(qbertCurse));
	scene.Add(std::move(controlDisplay));
	auto qbertTemp = scene.Add(std::move(qbert1));
	auto coilyTemp = scene.Add(std::move(coily));

	// Temp way of adding qbert to the level
	levelManagerTemp->GetComponent<LevelManagerComponent>()->SpawnQbert(qbertTemp);
	levelManagerTemp->GetComponent<LevelManagerComponent>()->SpawnCoily(coilyTemp);
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

	dae::Minigin engine{};
	engine.Run(load);

#ifdef STEAM
	SteamAPI_Shutdown();
#endif // Steam
    return 0;
}