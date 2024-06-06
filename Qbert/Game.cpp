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
#include "TitleComponent.h"
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
	case SceneType::Coop:
	case SceneType::Versus:
		LoadLevel(scene);
		break;
	case SceneType::Highscore:

		break;
	default:
		return;
	}
	m_CurrentSceneType = scene;
	dae::SceneManager::GetInstance().Init();
}

void Game::LoadMainMenu()
{
	auto& soundSystem = dae::ServiceLocator::GetSoundSystem();
	auto& input = dae::InputManager::GetInstance();
	auto& sceneManager = dae::SceneManager::GetInstance();

	glm::ivec2 windowSize = dae::Minigin::m_WindowSize;

	std::string sceneName = "MainMenu";
	auto& scene = sceneManager.CreateScene(sceneName);
	sceneManager.SetCurrentScene(sceneName);

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
}

void Game::LoadLevel(SceneType sceneType)
{
	auto& input = dae::InputManager::GetInstance();
	auto& resourceManager = dae::ServiceLocator::GetResourceManager();
	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& soundSystem = dae::ServiceLocator::GetSoundSystem();

	glm::ivec2 windowSize = dae::Minigin::m_WindowSize;

	std::string sceneName = "Level";
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

	auto font = resourceManager.LoadFont("Minecraft.ttf", 36);

	// player1Text
	auto player1Text = std::make_unique<dae::GameObject>();
	player1Text->AddComponent<dae::GraphicsComponent>("Player Titles.png");
	player1Text->AddComponent<dae::SpritesheetComponent>(1,2);
	player1Text->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(0, 0);
	player1Text->SetPosition(10,10);
	player1Text->SetScale(3,3);
	scene.Add(std::move(player1Text));

	if (sceneType == SceneType::Coop)
	{
		auto player2Text = std::make_unique<dae::GameObject>();
		player2Text->AddComponent<dae::GraphicsComponent>("Player Titles.png");
		player2Text->AddComponent<dae::SpritesheetComponent>(1, 2);
		player2Text->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(0, 1);
		player2Text->SetScale(3, 3);
		player2Text->SetPosition
		(
			dae::Minigin::m_WindowSize.x - player2Text->GetComponent<dae::GraphicsComponent>()->GetTextureSize().x - 10.f
			, 10.f
		);
		scene.Add(std::move(player2Text));
	}

	const glm::vec3 scale{ 2.f,2.f,0.f };

	// Tiles
	auto levelManager = std::make_unique<dae::GameObject>();
	levelManager->SetPosition(285, 95);
	levelManager->SetScale(scale);
	levelManager->AddComponent<LevelManagerComponent>();
	auto pLevelManagerComponent = levelManager->GetComponent<LevelManagerComponent>();

	MovementInfo::AddMovementInfo(MovementDirection::Up, glm::vec3(16, -24, 0) * scale, glm::ivec2{ 0,-1 });
	MovementInfo::AddMovementInfo(MovementDirection::Left, glm::vec3(-16, -24, 0) * scale, glm::ivec2{ -1, 0 });
	MovementInfo::AddMovementInfo(MovementDirection::Down, glm::vec3(-16, 24, 0) * scale, glm::ivec2{ 0, 1 });
	MovementInfo::AddMovementInfo(MovementDirection::Right, glm::vec3(16, 24, 0) * scale, glm::ivec2{ 1, 0 });
	MovementInfo::AddMovementInfo(MovementDirection::DiagonalLeft, glm::vec3(-32, 0, 0) * scale, glm::ivec2{ -1, 1 });
	MovementInfo::AddMovementInfo(MovementDirection::DiagonalRight, glm::vec3(32, 0, 0) * scale, glm::ivec2{ 1,-1 });

	// Qbert
	std::vector<std::unique_ptr<dae::GameObject>> vQbert;
	std::vector<std::unique_ptr<dae::GameObject>> vQbertCurse;

	vQbert.push_back(std::make_unique<dae::GameObject>());
	auto qbert1 = vQbert.back().get();
	qbert1->AddComponent<dae::GraphicsComponent>("Qbert P1 Spritesheet.png");
	qbert1->AddComponent<QbertComponent>(pLevelManagerComponent);
	qbert1->AddComponent<dae::SpritesheetComponent>(4, 1);
	qbert1->SetScale(scale);
	uint8_t controllerId = (sceneType == SceneType::Solo) ? 0 : 1;
	input.BindCommand(std::make_unique<MoveCommand>(qbert1, MovementInfo::GetMovementInfo(MovementDirection::Up)),		SDL_SCANCODE_W, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1, MovementInfo::GetMovementInfo(MovementDirection::Left)),	SDL_SCANCODE_A, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1, MovementInfo::GetMovementInfo(MovementDirection::Down)),	SDL_SCANCODE_S, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1, MovementInfo::GetMovementInfo(MovementDirection::Right)),	SDL_SCANCODE_D, dae::InputType::Down);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1, MovementInfo::GetMovementInfo(MovementDirection::Up)),		dae::ControllerButton::DPAD_UP,		dae::InputType::Down, controllerId);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1, MovementInfo::GetMovementInfo(MovementDirection::Left)),	dae::ControllerButton::DPAD_LEFT,	dae::InputType::Down, controllerId);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1, MovementInfo::GetMovementInfo(MovementDirection::Down)),	dae::ControllerButton::DPAD_DOWN,	dae::InputType::Down, controllerId);
	input.BindCommand(std::make_unique<MoveCommand>(qbert1, MovementInfo::GetMovementInfo(MovementDirection::Right)),	dae::ControllerButton::DPAD_RIGHT,	dae::InputType::Down, controllerId);
	if (sceneType == SceneType::Coop)
		qbert1->GetComponent<QbertComponent>()->SetSpawnPositions({ { 0,6 } });
	else
		qbert1->GetComponent<QbertComponent>()->SetSpawnPositions({ { 0,0 } });

	// Qbert curse
	vQbertCurse.push_back(std::make_unique<dae::GameObject>());
	auto qbertCurse1 = vQbertCurse.back().get();
	qbertCurse1->AddComponent<dae::GraphicsComponent>("Qbert Curses.png", false);
	qbertCurse1->AddComponent<QbertCurseComponent>();
	qbertCurse1->SetParent(qbert1);

	if (sceneType == SceneType::Coop)
	{
		// Qbert2
		vQbert.push_back(std::make_unique<dae::GameObject>());
		auto qbert2 = vQbert.back().get();
		qbert2->AddComponent<dae::GraphicsComponent>("Qbert P2 Spritesheet.png");
		qbert2->AddComponent<QbertComponent>(pLevelManagerComponent, Character::Qbert2);
		qbert2->AddComponent<dae::SpritesheetComponent>(4, 1);
		qbert2->SetScale(scale);
		controllerId = (sceneType == SceneType::Solo) ? 1 : 0;
		input.BindCommand(std::make_unique<MoveCommand>(qbert2, MovementInfo::GetMovementInfo(MovementDirection::Up)),		SDL_SCANCODE_UP,	dae::InputType::Down);
		input.BindCommand(std::make_unique<MoveCommand>(qbert2, MovementInfo::GetMovementInfo(MovementDirection::Left)),	SDL_SCANCODE_LEFT,	dae::InputType::Down);
		input.BindCommand(std::make_unique<MoveCommand>(qbert2, MovementInfo::GetMovementInfo(MovementDirection::Down)),	SDL_SCANCODE_DOWN,	dae::InputType::Down);
		input.BindCommand(std::make_unique<MoveCommand>(qbert2, MovementInfo::GetMovementInfo(MovementDirection::Right)),	SDL_SCANCODE_RIGHT, dae::InputType::Down);
		input.BindCommand(std::make_unique<MoveCommand>(qbert2, MovementInfo::GetMovementInfo(MovementDirection::Up)),		dae::ControllerButton::DPAD_UP,		dae::InputType::Down, controllerId);
		input.BindCommand(std::make_unique<MoveCommand>(qbert2, MovementInfo::GetMovementInfo(MovementDirection::Left)),	dae::ControllerButton::DPAD_LEFT,	dae::InputType::Down, controllerId);
		input.BindCommand(std::make_unique<MoveCommand>(qbert2, MovementInfo::GetMovementInfo(MovementDirection::Down)),	dae::ControllerButton::DPAD_DOWN,	dae::InputType::Down, controllerId);
		input.BindCommand(std::make_unique<MoveCommand>(qbert2, MovementInfo::GetMovementInfo(MovementDirection::Right)),	dae::ControllerButton::DPAD_RIGHT,	dae::InputType::Down, controllerId);
		qbert2->GetComponent<QbertComponent>()->SetSpawnPositions({ { 6,0 } });

		// Qbert2 curse
		vQbertCurse.push_back(std::make_unique<dae::GameObject>());
		auto& qbert2Curse = vQbertCurse.back();
		qbert2Curse->AddComponent<dae::GraphicsComponent>("Qbert Curses.png", false);
		qbert2Curse->AddComponent<QbertCurseComponent>();
		qbert2Curse->SetParent(qbert2);
	}

	// Coily
	auto coily = std::make_unique<dae::GameObject>();
	coily->AddComponent<dae::GraphicsComponent>("Coily Spritesheet.png");
	coily->AddComponent<CoilyComponent>(pLevelManagerComponent);
	coily->AddComponent<dae::SpritesheetComponent>(5, 2);
	coily->SetScale(scale);
	if (sceneType == SceneType::Versus)
	{
		input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Up)), SDL_SCANCODE_UP, dae::InputType::Down);
		input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Left)), SDL_SCANCODE_LEFT, dae::InputType::Down);
		input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Down)), SDL_SCANCODE_DOWN, dae::InputType::Down);
		input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Right)), SDL_SCANCODE_RIGHT, dae::InputType::Down);
		input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Up)), dae::ControllerButton::DPAD_UP, dae::InputType::Down, 0);
		input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Left)), dae::ControllerButton::DPAD_LEFT, dae::InputType::Down, 0);
		input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Down)), dae::ControllerButton::DPAD_DOWN, dae::InputType::Down, 0);
		input.BindCommand(std::make_unique<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Right)), dae::ControllerButton::DPAD_RIGHT, dae::InputType::Down, 0);
	}

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


	std::vector<std::unique_ptr<dae::GameObject>> vLives;
	std::vector<std::unique_ptr<dae::GameObject>> vScoreDisplay;
	// Lives 1
	vLives.push_back(std::make_unique<dae::GameObject>());
	auto lives1 = vLives.back().get();
	lives1->SetScale(scale);
	lives1->SetPosition(10.f, 80.f);
	lives1->AddComponent<LivesComponent>();

	// Score 1
	vScoreDisplay.push_back(std::make_unique<dae::GameObject>());
	auto scoreDisplay1 = vScoreDisplay.back().get();
	scoreDisplay1->AddComponent<dae::GraphicsComponent>();
	scoreDisplay1->AddComponent<dae::TextComponent>(font);
	scoreDisplay1->AddComponent<ScoreComponent>(pLevelManagerComponent);
	scoreDisplay1->SetPosition(10, 50);

	if (sceneType == SceneType::Coop)
	{
		// Lives 2
		vLives.push_back(std::make_unique<dae::GameObject>());
		auto lives2 = vLives.back().get();
		lives2->SetScale(scale);
		lives2->SetPosition(windowSize.x - 38.f, 80.f);
		lives2->AddComponent<LivesComponent>();

		// Score 2
		vScoreDisplay.push_back(std::make_unique<dae::GameObject>());
		auto scoreDisplay2 = vScoreDisplay.back().get();
		scoreDisplay2->AddComponent<dae::GraphicsComponent>();
		scoreDisplay2->AddComponent<dae::TextComponent>(font);
		scoreDisplay2->AddComponent<ScoreComponent>(pLevelManagerComponent, Character::Qbert2);
		scoreDisplay2->SetPosition(
			dae::Minigin::m_WindowSize.x - 150.f
			, 50.f
		);
	}

	// Titlescreen
	auto title = std::make_unique<dae::GameObject>();
	title->AddComponent<dae::GraphicsComponent>("Level Titles.png");
	title->AddComponent<dae::SpritesheetComponent>(1, 3);
	title->AddComponent<TitleComponent>(levelManager->GetComponent<LevelManagerComponent>());

	// Observers
	for (int i{}; i < vQbert.size(); ++i)
	{
		auto pQbertComponent = vQbert[i]->GetComponent<QbertComponent>();
		vLives[i]->GetComponent<LivesComponent>()->AddObserver(pQbertComponent->PlayerDied.get());
		vQbertCurse[i]->GetComponent<QbertCurseComponent>()->AddObserver(pQbertComponent->PlayerDied.get());
	}


	// Add to scene
	scene.Add(std::move(levelManager));
	for (auto& scoreDisplay : vScoreDisplay)
		scene.Add(std::move(scoreDisplay));
	for (auto& lives : vLives)
	scene.Add(std::move(lives));
	for (auto& qbertCurse : vQbertCurse)
	scene.Add(std::move(qbertCurse));
	for (auto& qbert : vQbert)
	scene.Add(std::move(qbert));
	scene.Add(std::move(coily));
	scene.Add(std::move(slick));
	scene.Add(std::move(sam));
	scene.Add(std::move(ugg));
	scene.Add(std::move(wrongway));
	scene.Add(std::move(title));
}