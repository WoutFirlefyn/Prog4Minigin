#include <SDL.h>

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
#include "SlickSamComponent.h"
#include "UggWrongwayComponent.h"
#include "ScoreComponent.h"
#include "LivesComponent.h"
#include "TileComponent.h"
#include "SpritesheetComponent.h"
#include "QbertCurseComponent.h"
#include "LevelManagerComponent.h"
#include "MainMenuComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"
#include "Game.h"

void Game::SetScene(SceneType scene)
{
	dae::InputManager::GetInstance().ClearInputActions();
	dae::ServiceLocator::GetSoundSystem().ClearSounds();
	switch (scene)
	{
	case SceneType::MainMenu:
		LoadMainMenu();
		break;
	case SceneType::Solo:
		LoadSolo();
		break;
	case SceneType::Coop:
		LoadCoop();
		break;
	case SceneType::Versus:
		LoadVersus();
		break;
	case SceneType::Highscore:

		break;
	default:
		return;
	}

	dae::SceneManager::GetInstance().GetCurrentScene().Init();
}

void Game::LoadMainMenu()
{
	auto& soundSystem = dae::ServiceLocator::GetSoundSystem();
	auto& input = dae::InputManager::GetInstance();
	auto& sceneManager = dae::SceneManager::GetInstance();

	glm::ivec2 windowSize = dae::Minigin::m_WindowSize;

	std::string sceneName = "MainMenu";
	auto& scene = sceneManager.CreateScene(sceneName);

	soundSystem.LoadSound("Change Selection.wav", dae::Sounds::ChangeSelection);

	auto title = std::make_unique<dae::GameObject>();
	title->AddComponent<dae::GraphicsComponent>("Game Title.png");
	glm::ivec2 titleSize = title->GetComponent<dae::GraphicsComponent>()->GetTextureSize();
	title->SetPosition((windowSize.x - titleSize.x) / 2.f, (windowSize.y / 1.5f - titleSize.y) / 2.f);
	scene.Add(std::move(title));

	auto arrow = std::make_unique<dae::GameObject>();
	arrow->AddComponent<dae::GraphicsComponent>("Selection Arrow.png");

	auto font = dae::ServiceLocator::GetResourceManager().LoadFont("Minecraft.ttf", 28);
	glm::vec4 textColor{ 250,190,80,255 };

	std::vector<dae::GameObject*> vMenuText{};

	auto menuText = std::make_unique<dae::GameObject>();
	menuText->AddComponent<dae::GraphicsComponent>();
	menuText->AddComponent<dae::TextComponent>(font, "Solo Mode");
	menuText->GetComponent<dae::TextComponent>()->SetColor(textColor);
	vMenuText.push_back(scene.Add(std::move(menuText)));

	menuText = std::make_unique<dae::GameObject>();
	menuText->AddComponent<dae::GraphicsComponent>();
	menuText->AddComponent<dae::TextComponent>(font, "Co-op Mode");
	menuText->GetComponent<dae::TextComponent>()->SetColor(textColor);
	vMenuText.push_back(scene.Add(std::move(menuText)));

	menuText = std::make_unique<dae::GameObject>();
	menuText->AddComponent<dae::GraphicsComponent>();
	menuText->AddComponent<dae::TextComponent>(font, "Versus Mode");
	menuText->GetComponent<dae::TextComponent>()->SetColor(textColor);
	vMenuText.push_back(scene.Add(std::move(menuText)));

	auto menuModeSelection = std::make_unique<dae::GameObject>();
	menuModeSelection->AddComponent<MainMenuComponent>(vMenuText, scene.Add(std::move(arrow)));
	menuModeSelection->SetPosition(windowSize.x / 2.f, windowSize.y / 1.6f);
	input.BindCommand(std::make_unique<ChangeGamemodeCommand>(menuModeSelection.get(), -1), SDL_SCANCODE_W, dae::InputType::Pressed);
	input.BindCommand(std::make_unique<ChangeGamemodeCommand>(menuModeSelection.get(), 1), SDL_SCANCODE_S, dae::InputType::Pressed);
	input.BindCommand(std::make_unique<ChangeGamemodeCommand>(menuModeSelection.get(), -1), dae::ControllerButton::DPAD_UP, dae::InputType::Pressed, 0);
	input.BindCommand(std::make_unique<ChangeGamemodeCommand>(menuModeSelection.get(), 1), dae::ControllerButton::DPAD_DOWN, dae::InputType::Pressed, 0);
	input.BindCommand(std::make_unique<ChangeGamemodeCommand>(menuModeSelection.get(), -1), dae::ControllerButton::DPAD_UP, dae::InputType::Pressed, 1);
	input.BindCommand(std::make_unique<ChangeGamemodeCommand>(menuModeSelection.get(), 1), dae::ControllerButton::DPAD_DOWN, dae::InputType::Pressed, 1);
	input.BindCommand(std::make_unique<SelectGamemodeCommand>(menuModeSelection.get()), SDL_SCANCODE_RETURN, dae::InputType::Pressed);
	input.BindCommand(std::make_unique<SelectGamemodeCommand>(menuModeSelection.get()), dae::ControllerButton::A, dae::InputType::Pressed, 0);
	input.BindCommand(std::make_unique<SelectGamemodeCommand>(menuModeSelection.get()), dae::ControllerButton::A, dae::InputType::Pressed, 1);
	scene.Add(std::move(menuModeSelection));

	sceneManager.SetCurrentScene(sceneName);
}

void Game::LoadSolo()
{
	auto& input = dae::InputManager::GetInstance();
	auto& resourceManager = dae::ServiceLocator::GetResourceManager();
	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& soundSystem = dae::ServiceLocator::GetSoundSystem();

	glm::ivec2 windowSize = dae::Minigin::m_WindowSize;

	std::string sceneName = "Solo";
	auto& scene = sceneManager.CreateScene(sceneName);

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

	auto font = resourceManager.LoadFont("Minecraft.ttf", 36);

	// FPS
	auto fps = std::make_unique<dae::GameObject>();
	fps->AddComponent<dae::GraphicsComponent>();
	fps->AddComponent<dae::TextComponent>(font);
	fps->AddComponent<FPSComponent>();
	fps->SetPosition(10, 10);

	const glm::vec3 scale{ 2.f,2.f,0.f };

	// Tiles
	auto levelManager = std::make_unique<dae::GameObject>();
	levelManager->AddComponent<LevelManagerComponent>(scene);
	levelManager->SetPosition(285, 95);
	levelManager->SetScale(scale);
	auto pLevelManagerComponent = levelManager->GetComponent<LevelManagerComponent>();

	MovementInfo::AddMovementInfo(MovementDirection::Up, glm::vec3(16, -24, 0) * scale, glm::ivec2{ 0,-1 });
	MovementInfo::AddMovementInfo(MovementDirection::Left, glm::vec3(-16, -24, 0) * scale, glm::ivec2{ -1, 0 });
	MovementInfo::AddMovementInfo(MovementDirection::Down, glm::vec3(-16, 24, 0) * scale, glm::ivec2{ 0, 1 });
	MovementInfo::AddMovementInfo(MovementDirection::Right, glm::vec3(16, 24, 0) * scale, glm::ivec2{ 1, 0 });
	MovementInfo::AddMovementInfo(MovementDirection::DiagonalLeft, glm::vec3(-32, 0, 0) * scale, glm::ivec2{ -1, 1 });
	MovementInfo::AddMovementInfo(MovementDirection::DiagonalRight, glm::vec3(32, 0, 0) * scale, glm::ivec2{ 1,-1 });

	// Qbert1
	auto qbert1 = std::make_unique<dae::GameObject>();
	qbert1->AddComponent<dae::GraphicsComponent>("Qbert P1 Spritesheet.png");
	qbert1->AddComponent<QbertComponent>(pLevelManagerComponent);
	qbert1->AddComponent<dae::SpritesheetComponent>(4, 1);
	qbert1->SetScale(scale);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementInfo::GetMovementInfo(MovementDirection::Up)), SDL_SCANCODE_W, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementInfo::GetMovementInfo(MovementDirection::Left)), SDL_SCANCODE_A, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementInfo::GetMovementInfo(MovementDirection::Down)), SDL_SCANCODE_S, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementInfo::GetMovementInfo(MovementDirection::Right)), SDL_SCANCODE_D, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementInfo::GetMovementInfo(MovementDirection::Up)), dae::ControllerButton::DPAD_UP, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementInfo::GetMovementInfo(MovementDirection::Left)), dae::ControllerButton::DPAD_LEFT, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementInfo::GetMovementInfo(MovementDirection::Down)), dae::ControllerButton::DPAD_DOWN, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementInfo::GetMovementInfo(MovementDirection::Right)), dae::ControllerButton::DPAD_RIGHT, dae::InputType::Down);

	// Qbert curse
	auto qbertCurse = std::make_unique<dae::GameObject>();
	qbertCurse->AddComponent<dae::GraphicsComponent>("Qbert Curses.png", false);
	qbertCurse->AddComponent<QbertCurseComponent>();
	qbertCurse->SetParent(qbert1.get());

	// Coily
	auto coily = std::make_unique<dae::GameObject>();
	coily->AddComponent<dae::GraphicsComponent>("Coily Spritesheet.png");
	coily->AddComponent<CoilyComponent>(pLevelManagerComponent);
	coily->AddComponent<dae::SpritesheetComponent>(5, 2);
	coily->SetScale(scale);

	// Slick
	auto slick = std::make_unique<dae::GameObject>();
	slick->AddComponent<dae::GraphicsComponent>("Slick Sam Spritesheet.png");
	slick->AddComponent<SlickSamComponent>(Character::Slick, pLevelManagerComponent);
	slick->AddComponent<dae::SpritesheetComponent>(2, 2);
	slick->SetScale(scale);

	// Sam
	auto sam = std::make_unique<dae::GameObject>();
	sam->AddComponent<dae::GraphicsComponent>("Slick Sam Spritesheet.png");
	sam->AddComponent<SlickSamComponent>(Character::Sam, pLevelManagerComponent);
	sam->AddComponent<dae::SpritesheetComponent>(2, 2);
	sam->SetScale(scale);

	// Ugg
	auto ugg = std::make_unique<dae::GameObject>();
	ugg->AddComponent<dae::GraphicsComponent>("Ugg Wrongway Spritesheet.png");
	ugg->AddComponent<UggWrongwayComponent>(Character::Ugg, pLevelManagerComponent);
	ugg->AddComponent<dae::SpritesheetComponent>(4, 2);
	ugg->SetScale(scale);

	// Wrongway
	auto wrongway = std::make_unique<dae::GameObject>();
	wrongway->AddComponent<dae::GraphicsComponent>("Ugg Wrongway Spritesheet.png");
	wrongway->AddComponent<UggWrongwayComponent>(Character::Wrongway, pLevelManagerComponent);
	wrongway->AddComponent<dae::SpritesheetComponent>(4, 2);
	wrongway->SetScale(scale);

	font = resourceManager.LoadFont("Minecraft.ttf", 20);

	// Lives 1
	std::vector<std::unique_ptr<dae::GameObject>> vHearts{};

	for (size_t i{}; i < 3; ++i)
	{
		vHearts.push_back(std::make_unique<dae::GameObject>());
		vHearts[i]->AddComponent<dae::GraphicsComponent>("Heart.png");
		vHearts[i]->AddComponent<LivesComponent>();
		vHearts[i]->SetScale(scale);
		vHearts[i]->SetPosition(10, 80 + 18 * i * scale.y);
	}

	// Score 1
	auto scoreDisplay = std::make_unique<dae::GameObject>();
	scoreDisplay->AddComponent<dae::GraphicsComponent>();
	scoreDisplay->AddComponent<dae::TextComponent>(font);
	scoreDisplay->AddComponent<ScoreComponent>(pLevelManagerComponent);
	scoreDisplay->SetPosition(10, 50);

	// Observers
	auto pQbertComponent1 = qbert1->GetComponent<QbertComponent>();

	for (auto& heart : vHearts)
		heart->GetComponent<LivesComponent>()->AddObserver(pQbertComponent1->PlayerDied.get());
	qbertCurse->GetComponent<QbertCurseComponent>()->AddObserver(pQbertComponent1->PlayerDied.get());

	// Add to scene
	scene.Add(std::move(levelManager));
	scene.Add(std::move(fps));
	scene.Add(std::move(scoreDisplay));
	for (auto& heart : vHearts)
		scene.Add(std::move(heart));
	scene.Add(std::move(qbertCurse));
	scene.Add(std::move(qbert1));
	scene.Add(std::move(coily));
	scene.Add(std::move(slick));
	scene.Add(std::move(sam));
	scene.Add(std::move(ugg));
	scene.Add(std::move(wrongway));

	sceneManager.SetCurrentScene(sceneName);
}

void Game::LoadCoop()
{
}

void Game::LoadVersus()
{
	auto& input = dae::InputManager::GetInstance();
	auto& resourceManager = dae::ServiceLocator::GetResourceManager();
	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& soundSystem = dae::ServiceLocator::GetSoundSystem();

	glm::ivec2 windowSize = dae::Minigin::m_WindowSize;

	std::string sceneName = "Versus";
	auto& scene = sceneManager.CreateScene(sceneName);
	sceneManager.SetCurrentScene(sceneName);

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

	// Background
	auto backGround = std::make_unique<dae::GameObject>();
	backGround->AddComponent<dae::GraphicsComponent>("background.jpg");
	scene.Add(std::move(backGround));

	auto font = resourceManager.LoadFont("Minecraft.ttf", 36);

	// FPS
	auto fps = std::make_unique<dae::GameObject>();
	fps->AddComponent<dae::GraphicsComponent>();
	fps->AddComponent<dae::TextComponent>(font);
	fps->AddComponent<FPSComponent>();
	fps->SetPosition(10, 10);

	const glm::vec3 scale{ 2.f,2.f,0.f };

	// Tiles
	auto levelManager = std::make_unique<dae::GameObject>();
	levelManager->AddComponent<LevelManagerComponent>(scene);
	levelManager->SetPosition(285, 95);
	levelManager->SetScale(scale);
	auto pLevelManagerComponent = levelManager->GetComponent<LevelManagerComponent>();

	MovementInfo::AddMovementInfo(MovementDirection::Up, glm::vec3(16, -24, 0) * scale, glm::ivec2{ 0,-1 });
	MovementInfo::AddMovementInfo(MovementDirection::Left, glm::vec3(-16, -24, 0) * scale, glm::ivec2{ -1, 0 });
	MovementInfo::AddMovementInfo(MovementDirection::Down, glm::vec3(-16, 24, 0) * scale, glm::ivec2{ 0, 1 });
	MovementInfo::AddMovementInfo(MovementDirection::Right, glm::vec3(16, 24, 0) * scale, glm::ivec2{ 1, 0 });
	MovementInfo::AddMovementInfo(MovementDirection::DiagonalLeft, glm::vec3(-32, 0, 0) * scale, glm::ivec2{ -1, 1 });
	MovementInfo::AddMovementInfo(MovementDirection::DiagonalRight, glm::vec3(32, 0, 0) * scale, glm::ivec2{  1,-1 });

	// Qbert1
	auto qbert1 = std::make_unique<dae::GameObject>();
	qbert1->AddComponent<dae::GraphicsComponent>("Qbert P1 Spritesheet.png");
	qbert1->AddComponent<QbertComponent>(pLevelManagerComponent);
	qbert1->AddComponent<dae::SpritesheetComponent>(4, 1);
	qbert1->SetScale(scale);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementInfo::GetMovementInfo(MovementDirection::Up)), SDL_SCANCODE_W, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementInfo::GetMovementInfo(MovementDirection::Left)), SDL_SCANCODE_A, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementInfo::GetMovementInfo(MovementDirection::Down)), SDL_SCANCODE_S, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementInfo::GetMovementInfo(MovementDirection::Right)), SDL_SCANCODE_D, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementInfo::GetMovementInfo(MovementDirection::Up)), dae::ControllerButton::DPAD_UP, dae::InputType::Down, 1);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementInfo::GetMovementInfo(MovementDirection::Left)), dae::ControllerButton::DPAD_LEFT, dae::InputType::Down, 1);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementInfo::GetMovementInfo(MovementDirection::Down)), dae::ControllerButton::DPAD_DOWN, dae::InputType::Down, 1);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1.get(), MovementInfo::GetMovementInfo(MovementDirection::Right)), dae::ControllerButton::DPAD_RIGHT, dae::InputType::Down, 1);

	// Qbert curse
	auto qbertCurse = std::make_unique<dae::GameObject>();
	qbertCurse->AddComponent<dae::GraphicsComponent>("Qbert Curses.png", false);
	qbertCurse->AddComponent<QbertCurseComponent>();
	qbertCurse->SetParent(qbert1.get());

	// Coily
	auto coily = std::make_unique<dae::GameObject>();
	coily->AddComponent<dae::GraphicsComponent>("Coily Spritesheet.png");
	coily->AddComponent<CoilyComponent>(pLevelManagerComponent, true);
	coily->AddComponent<dae::SpritesheetComponent>(5, 2);
	coily->SetScale(scale);
	input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Up)), SDL_SCANCODE_UP, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Left)), SDL_SCANCODE_LEFT, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Down)), SDL_SCANCODE_DOWN, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Right)), SDL_SCANCODE_RIGHT, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Up)), dae::ControllerButton::DPAD_UP, dae::InputType::Down, 0);
	input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Left)), dae::ControllerButton::DPAD_LEFT, dae::InputType::Down, 0);
	input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Down)), dae::ControllerButton::DPAD_DOWN, dae::InputType::Down, 0);
	input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Right)), dae::ControllerButton::DPAD_RIGHT, dae::InputType::Down, 0);

	// Slick
	auto slick = std::make_unique<dae::GameObject>();
	slick->AddComponent<dae::GraphicsComponent>("Slick Sam Spritesheet.png");
	slick->AddComponent<SlickSamComponent>(Character::Slick, pLevelManagerComponent);
	slick->AddComponent<dae::SpritesheetComponent>(2, 2);
	slick->SetScale(scale);

	// Sam
	auto sam = std::make_unique<dae::GameObject>();
	sam->AddComponent<dae::GraphicsComponent>("Slick Sam Spritesheet.png");
	sam->AddComponent<SlickSamComponent>(Character::Sam, pLevelManagerComponent);
	sam->AddComponent<dae::SpritesheetComponent>(2, 2);
	sam->SetScale(scale);

	// Ugg
	auto ugg = std::make_unique<dae::GameObject>();
	ugg->AddComponent<dae::GraphicsComponent>("Ugg Wrongway Spritesheet.png");
	ugg->AddComponent<UggWrongwayComponent>(Character::Ugg, pLevelManagerComponent);
	ugg->AddComponent<dae::SpritesheetComponent>(4, 2);
	ugg->SetScale(scale);

	// Wrongway
	auto wrongway = std::make_unique<dae::GameObject>();
	wrongway->AddComponent<dae::GraphicsComponent>("Ugg Wrongway Spritesheet.png");
	wrongway->AddComponent<UggWrongwayComponent>(Character::Wrongway, pLevelManagerComponent);
	wrongway->AddComponent<dae::SpritesheetComponent>(4, 2);
	wrongway->SetScale(scale);

	font = resourceManager.LoadFont("Minecraft.ttf", 20);

	// Lives 1
	std::vector<std::unique_ptr<dae::GameObject>> vHearts{};

	for (size_t i{}; i < 3; ++i)
	{
		vHearts.push_back(std::make_unique<dae::GameObject>());
		vHearts[i]->AddComponent<dae::GraphicsComponent>("Heart.png");
		vHearts[i]->AddComponent<LivesComponent>();
		vHearts[i]->SetScale(scale);
		vHearts[i]->SetPosition(10, 80 + 18 * i * scale.y);
	}

	// Score 1
	auto scoreDisplay = std::make_unique<dae::GameObject>();
	scoreDisplay->AddComponent<dae::GraphicsComponent>();
	scoreDisplay->AddComponent<dae::TextComponent>(font);
	scoreDisplay->AddComponent<ScoreComponent>(pLevelManagerComponent);
	scoreDisplay->SetPosition(10, 50);

	// Observers
	auto pQbertComponent1 = qbert1->GetComponent<QbertComponent>();

	for (auto& heart : vHearts)
		heart->GetComponent<LivesComponent>()->AddObserver(pQbertComponent1->PlayerDied.get());
	qbertCurse->GetComponent<QbertCurseComponent>()->AddObserver(pQbertComponent1->PlayerDied.get());

	// Add to scene
	scene.Add(std::move(levelManager));
	scene.Add(std::move(fps));
	scene.Add(std::move(scoreDisplay));
	for (auto& heart : vHearts)
		scene.Add(std::move(heart));
	scene.Add(std::move(qbertCurse));
	scene.Add(std::move(qbert1));
	scene.Add(std::move(coily));
	scene.Add(std::move(slick));
	scene.Add(std::move(sam));
	scene.Add(std::move(ugg));
	scene.Add(std::move(wrongway));
}
