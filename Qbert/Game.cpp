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
#include "HighScoreComponent.h"
#include "TitleComponent.h"
#include "RoundCounterComponent.h"
#include "TileIconComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"
#include "Game.h"
#include "Utils.h"
#include <fstream>

using json = nlohmann::json;

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
		LoadHighscoreScreen();
		break;
	case SceneType::GameOver:
	case SceneType::Win:
		LoadEndScreen(scene);
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

	auto font = dae::ServiceLocator::GetResourceManager().LoadFont("Monocraft.ttf", 28);
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

	menuText = std::make_unique<dae::GameObject>();
	menuText->AddComponent<dae::GraphicsComponent>();
	menuText->AddComponent<dae::TextComponent>(font, "Highscores");
	menuText->GetComponent<dae::TextComponent>()->SetColor(textColor);
	vMenuText.push_back(scene.Add(std::move(menuText)));

	auto menuModeSelection = std::make_unique<dae::GameObject>();
	menuModeSelection->AddComponent<MainMenuComponent>(vMenuText, scene.Add(std::move(arrow)));
	menuModeSelection->SetPosition(windowSize.x / 2.f, windowSize.y / 1.8f);

	auto prevModeCommand = std::make_shared<ChangeGamemodeCommand>(menuModeSelection.get(), -1);
	auto nextModeCommand = std::make_shared<ChangeGamemodeCommand>(menuModeSelection.get(),  1);
	auto selectModeCommand = std::make_shared<SelectGamemodeCommand>(menuModeSelection.get());
	input.BindCommand(prevModeCommand, SDL_SCANCODE_W, dae::InputType::Pressed);
	input.BindCommand(nextModeCommand, SDL_SCANCODE_S, dae::InputType::Pressed);
	input.BindCommand(prevModeCommand, dae::ControllerButton::DPAD_UP, dae::InputType::Pressed, 0);
	input.BindCommand(nextModeCommand, dae::ControllerButton::DPAD_DOWN, dae::InputType::Pressed, 0);
	input.BindCommand(prevModeCommand, dae::ControllerButton::DPAD_UP, dae::InputType::Pressed, 1);
	input.BindCommand(nextModeCommand, dae::ControllerButton::DPAD_DOWN, dae::InputType::Pressed, 1);
	input.BindCommand(selectModeCommand, SDL_SCANCODE_RETURN, dae::InputType::Released);
	input.BindCommand(selectModeCommand, dae::ControllerButton::A, dae::InputType::Released, 0);
	input.BindCommand(selectModeCommand, dae::ControllerButton::A, dae::InputType::Released, 1);
	scene.Add(std::move(menuModeSelection));
}

void Game::LoadLevel(SceneType sceneType)
{
	auto& input = dae::InputManager::GetInstance();
	auto& resourceManager = dae::ServiceLocator::GetResourceManager();
	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& soundSystem = dae::ServiceLocator::GetSoundSystem();

	glm::ivec2 windowSize = dae::Minigin::m_WindowSize;

	std::string sceneName{};

	switch (sceneType)
	{
	case SceneType::Solo:
		sceneName = "Solo";
		break;
	case SceneType::Coop:
		sceneName = "Coop";
		break;
	case SceneType::Versus:
		sceneName = "Versus";
		break;
	default:
		return;
	}

	auto& scene = sceneManager.CreateScene(sceneName);
	sceneManager.SetCurrentScene(sceneName);

	auto vSpawnData = GetSpawnData();

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

	input.BindCommand(std::make_unique<ToggleSoundCommand>(), SDL_SCANCODE_M, dae::InputType::Released);

	auto font = resourceManager.LoadFont("Monocraft.ttf", 36);
	glm::vec4 textColor{ 255,200,90,255 };

	// playerText
	auto playerText = std::make_unique<dae::GameObject>();
	playerText->AddComponent<dae::GraphicsComponent>("Player Titles.png");
	playerText->AddComponent<dae::SpritesheetComponent>(1,2);
	playerText->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(0, 0);
	playerText->SetPosition(10,10);
	playerText->SetScale(3,3);
	scene.Add(std::move(playerText));

	if (sceneType == SceneType::Coop)
	{
		playerText = std::make_unique<dae::GameObject>();
		playerText->AddComponent<dae::GraphicsComponent>("Player Titles.png");
		playerText->AddComponent<dae::SpritesheetComponent>(1, 2);
		playerText->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(0, 1);
		playerText->SetScale(3, 3);
		playerText->SetPosition
		(
			dae::Minigin::m_WindowSize.x - playerText->GetComponent<dae::GraphicsComponent>()->GetTextureSize().x - 10.f, 
			10.f
		);
		scene.Add(std::move(playerText));
	}

	const glm::vec3 scale{ 2.f,2.f,0.f };

	// Tiles
	auto levelManager = std::make_unique<dae::GameObject>();
	levelManager->SetPosition(285, 95);
	levelManager->SetScale(scale);
	levelManager->AddComponent<LevelManagerComponent>();
	auto pLevelManagerComponent = levelManager->GetComponent<LevelManagerComponent>(); 
	input.BindCommand(std::make_unique<NextRoundCommand>(levelManager.get()), SDL_SCANCODE_F1, dae::InputType::Released);

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
	qbert1->GetComponent<QbertComponent>()->SetSpawnData(vSpawnData[static_cast<int>(Character::Qbert1)]);
	qbert1->AddComponent<dae::SpritesheetComponent>(4, 1);
	qbert1->SetScale(scale);
	uint8_t controllerId = (sceneType == SceneType::Solo) ? 0 : 1;

	auto moveUpCommand	  = std::make_shared<MoveCommand>(qbert1, MovementInfo::GetMovementInfo(MovementDirection::Up));
	auto moveLeftCommand  = std::make_shared<MoveCommand>(qbert1, MovementInfo::GetMovementInfo(MovementDirection::Left));
	auto moveDownCommand  = std::make_shared<MoveCommand>(qbert1, MovementInfo::GetMovementInfo(MovementDirection::Down));
	auto moveRightCommand = std::make_shared<MoveCommand>(qbert1, MovementInfo::GetMovementInfo(MovementDirection::Right));
	input.BindCommand(moveUpCommand,	SDL_SCANCODE_W, dae::InputType::Down);
	input.BindCommand(moveLeftCommand,	SDL_SCANCODE_A, dae::InputType::Down);
	input.BindCommand(moveDownCommand,	SDL_SCANCODE_S, dae::InputType::Down);
	input.BindCommand(moveRightCommand,	SDL_SCANCODE_D, dae::InputType::Down);
	input.BindCommand(moveUpCommand,	dae::ControllerButton::DPAD_UP,		dae::InputType::Down, controllerId);
	input.BindCommand(moveLeftCommand,	dae::ControllerButton::DPAD_LEFT,	dae::InputType::Down, controllerId);
	input.BindCommand(moveDownCommand,	dae::ControllerButton::DPAD_DOWN,	dae::InputType::Down, controllerId);
	input.BindCommand(moveRightCommand,	dae::ControllerButton::DPAD_RIGHT,	dae::InputType::Down, controllerId);

	// Qbert curse
	vQbertCurse.push_back(std::make_unique<dae::GameObject>());
	auto qbertCurse1 = vQbertCurse.back().get();
	qbertCurse1->AddComponent<dae::GraphicsComponent>("Qbert Curses.png", false);
	qbertCurse1->AddComponent<QbertCurseComponent>(qbert1->GetComponent<QbertComponent>());
	qbertCurse1->SetParent(qbert1);

	if (sceneType == SceneType::Coop)
	{
		// Qbert2
		vQbert.push_back(std::make_unique<dae::GameObject>());
		auto qbert2 = vQbert.back().get();
		qbert2->AddComponent<dae::GraphicsComponent>("Qbert P2 Spritesheet.png");
		qbert2->AddComponent<QbertComponent>(pLevelManagerComponent, Character::Qbert2);
		qbert2->GetComponent<QbertComponent>()->SetSpawnData(vSpawnData[static_cast<int>(Character::Qbert2)]);
		qbert2->AddComponent<dae::SpritesheetComponent>(4, 1);
		qbert2->SetScale(scale);
		controllerId = (sceneType == SceneType::Solo) ? 1 : 0;	
		moveUpCommand	 = std::make_shared<MoveCommand>(qbert2, MovementInfo::GetMovementInfo(MovementDirection::Up));
		moveLeftCommand	 = std::make_shared<MoveCommand>(qbert2, MovementInfo::GetMovementInfo(MovementDirection::Left));
		moveDownCommand	 = std::make_shared<MoveCommand>(qbert2, MovementInfo::GetMovementInfo(MovementDirection::Down));
		moveRightCommand = std::make_shared<MoveCommand>(qbert2, MovementInfo::GetMovementInfo(MovementDirection::Right));
		input.BindCommand(moveUpCommand,	SDL_SCANCODE_UP,	dae::InputType::Down);
		input.BindCommand(moveLeftCommand,	SDL_SCANCODE_LEFT,	dae::InputType::Down);
		input.BindCommand(moveDownCommand,	SDL_SCANCODE_DOWN,	dae::InputType::Down);
		input.BindCommand(moveRightCommand,	SDL_SCANCODE_RIGHT, dae::InputType::Down);
		input.BindCommand(moveUpCommand,	dae::ControllerButton::DPAD_UP,		dae::InputType::Down, controllerId);
		input.BindCommand(moveLeftCommand,	dae::ControllerButton::DPAD_LEFT,	dae::InputType::Down, controllerId);
		input.BindCommand(moveDownCommand,	dae::ControllerButton::DPAD_DOWN,	dae::InputType::Down, controllerId);
		input.BindCommand(moveRightCommand,	dae::ControllerButton::DPAD_RIGHT,	dae::InputType::Down, controllerId);

		// Qbert2 curse
		vQbertCurse.push_back(std::make_unique<dae::GameObject>());
		auto& qbert2Curse = vQbertCurse.back();
		qbert2Curse->AddComponent<dae::GraphicsComponent>("Qbert Curses.png", false);
		qbert2Curse->AddComponent<QbertCurseComponent>(qbert2->GetComponent<QbertComponent>());
		qbert2Curse->SetParent(qbert2);
	}

	// Coily
	auto coily = std::make_unique<dae::GameObject>();
	coily->AddComponent<dae::GraphicsComponent>("Coily Spritesheet.png");
	coily->AddComponent<CoilyComponent>(pLevelManagerComponent);
	coily->GetComponent<CoilyComponent>()->SetSpawnData(vSpawnData[static_cast<int>(Character::Coily)]);
	coily->AddComponent<dae::SpritesheetComponent>(5, 2);
	coily->SetScale(scale);
	if (sceneType == SceneType::Versus)
	{
		moveUpCommand    = std::make_shared<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Up));
		moveLeftCommand	 = std::make_shared<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Left));
		moveDownCommand	 = std::make_shared<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Down));
		moveRightCommand = std::make_shared<MoveCommand>(coily.get(), MovementInfo::GetMovementInfo(MovementDirection::Right));
		input.BindCommand(moveUpCommand,	SDL_SCANCODE_UP,	dae::InputType::Down);
		input.BindCommand(moveLeftCommand,	SDL_SCANCODE_LEFT,	dae::InputType::Down);
		input.BindCommand(moveDownCommand,	SDL_SCANCODE_DOWN,	dae::InputType::Down);
		input.BindCommand(moveRightCommand,	SDL_SCANCODE_RIGHT, dae::InputType::Down);
		input.BindCommand(moveUpCommand,	dae::ControllerButton::DPAD_UP,		dae::InputType::Down, 0);
		input.BindCommand(moveLeftCommand,	dae::ControllerButton::DPAD_LEFT,	dae::InputType::Down, 0);
		input.BindCommand(moveDownCommand,	dae::ControllerButton::DPAD_DOWN,	dae::InputType::Down, 0);
		input.BindCommand(moveRightCommand,	dae::ControllerButton::DPAD_RIGHT,	dae::InputType::Down, 0);
	}

	// Slick
	auto slick = std::make_unique<dae::GameObject>();
	slick->AddComponent<dae::GraphicsComponent>("Slick Sam Spritesheet.png");
	slick->AddComponent<SlickSamComponent>(Character::Slick, pLevelManagerComponent);
	slick->GetComponent<SlickSamComponent>()->SetSpawnData(vSpawnData[static_cast<int>(Character::Slick)]);
	slick->AddComponent<dae::SpritesheetComponent>(2, 2);
	slick->SetScale(scale);

	// Sam
	auto sam = std::make_unique<dae::GameObject>();
	sam->AddComponent<dae::GraphicsComponent>("Slick Sam Spritesheet.png");
	sam->AddComponent<SlickSamComponent>(Character::Sam, pLevelManagerComponent);
	sam->GetComponent<SlickSamComponent>()->SetSpawnData(vSpawnData[static_cast<int>(Character::Sam)]);
	sam->AddComponent<dae::SpritesheetComponent>(2, 2);
	sam->SetScale(scale);

	// Ugg
	auto ugg = std::make_unique<dae::GameObject>();
	ugg->AddComponent<dae::GraphicsComponent>("Ugg Wrongway Spritesheet.png");
	ugg->AddComponent<UggWrongwayComponent>(Character::Ugg, pLevelManagerComponent);
	ugg->GetComponent<UggWrongwayComponent>()->SetSpawnData(vSpawnData[static_cast<int>(Character::Ugg)]);
	ugg->AddComponent<dae::SpritesheetComponent>(4, 2);
	ugg->SetScale(scale);

	// Wrongway
	auto wrongway = std::make_unique<dae::GameObject>();
	wrongway->AddComponent<dae::GraphicsComponent>("Ugg Wrongway Spritesheet.png");
	wrongway->AddComponent<UggWrongwayComponent>(Character::Wrongway, pLevelManagerComponent);
	wrongway->GetComponent<UggWrongwayComponent>()->SetSpawnData(vSpawnData[static_cast<int>(Character::Wrongway)]);
	wrongway->AddComponent<dae::SpritesheetComponent>(4, 2);
	wrongway->SetScale(scale);

	font = resourceManager.LoadFont("Monocraft.ttf", 20);

	std::vector<std::unique_ptr<dae::GameObject>> vLives;
	std::vector<std::unique_ptr<dae::GameObject>> vScoreDisplay;
	// Lives 1
	vLives.push_back(std::make_unique<dae::GameObject>());
	auto lives1 = vLives.back().get();
	lives1->SetScale(scale);
	lives1->SetPosition(10.f, 80.f);
	lives1->AddComponent<LivesComponent>(pLevelManagerComponent, vQbert.front()->GetComponent<QbertComponent>());

	// Score 1
	vScoreDisplay.push_back(std::make_unique<dae::GameObject>());
	auto scoreDisplay1 = vScoreDisplay.back().get();
	scoreDisplay1->AddComponent<dae::GraphicsComponent>();
	scoreDisplay1->AddComponent<dae::TextComponent>(font);
	scoreDisplay1->GetComponent<dae::TextComponent>()->SetColor(textColor);
	scoreDisplay1->AddComponent<ScoreComponent>(pLevelManagerComponent);
	scoreDisplay1->SetPosition(10, 50);

	if (sceneType == SceneType::Coop)
	{
		// Lives 2
		vLives.push_back(std::make_unique<dae::GameObject>());
		auto lives2 = vLives.back().get();
		lives2->SetScale(scale);
		lives2->SetPosition(windowSize.x - 38.f, 80.f);
		lives2->AddComponent<LivesComponent>(pLevelManagerComponent, vQbert.back()->GetComponent<QbertComponent>());

		// Score 2
		vScoreDisplay.push_back(std::make_unique<dae::GameObject>());
		auto scoreDisplay2 = vScoreDisplay.back().get();
		scoreDisplay2->AddComponent<dae::GraphicsComponent>();
		scoreDisplay2->AddComponent<dae::TextComponent>(font);
		scoreDisplay2->GetComponent<dae::TextComponent>()->SetColor(textColor);
		scoreDisplay2->AddComponent<ScoreComponent>(pLevelManagerComponent, Character::Qbert2);
		scoreDisplay2->SetPosition(
			dae::Minigin::m_WindowSize.x - 150.f
			, 50.f
		);
	}

	font = resourceManager.LoadFont("Monocraft.ttf", 18);

	// Round / Level counter
	auto roundCounter = std::make_unique<dae::GameObject>();
	roundCounter->AddComponent<dae::GraphicsComponent>();
	roundCounter->AddComponent<dae::TextComponent>(font, "Level 0  Round 0");
	roundCounter->GetComponent<dae::TextComponent>()->SetColor(textColor);
	roundCounter->AddComponent<RoundCounterComponent>(pLevelManagerComponent);
	roundCounter->SetPosition((windowSize.x - roundCounter->GetComponent<dae::GraphicsComponent>()->GetTextureSize().x) / 2.f, 10.f);

	// Change to
	auto changeTo = std::make_unique<dae::GameObject>();
	changeTo->AddComponent<dae::GraphicsComponent>();
	changeTo->AddComponent<dae::TextComponent>(font, "Change To");
	changeTo->GetComponent<dae::TextComponent>()->SetColor(textColor);
	changeTo->SetPosition(10, 170);

	// Tile icon
	auto tileIcon = std::make_unique<dae::GameObject>();
	tileIcon->SetScale(scale);
	tileIcon->AddComponent<dae::GraphicsComponent>("Color Icons Spritesheet.png");
	tileIcon->AddComponent<dae::SpritesheetComponent>(6,2);
	tileIcon->AddComponent<TileIconComponent>(pLevelManagerComponent);
	tileIcon->SetParent(changeTo.get());
	glm::ivec2 changeToSize = changeTo->GetComponent<dae::GraphicsComponent>()->GetTextureSize();
	tileIcon->SetPosition(changeToSize.x + 15.f, (changeToSize.y - tileIcon->GetComponent<dae::GraphicsComponent>()->GetTextureSize().y) / 2.f);

	// Titlescreen
	auto title = std::make_unique<dae::GameObject>();
	title->AddComponent<dae::GraphicsComponent>("Level Titles.png");
	title->AddComponent<dae::SpritesheetComponent>(1, 3);
	title->AddComponent<TitleComponent>(levelManager->GetComponent<LevelManagerComponent>());

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
	scene.Add(std::move(roundCounter));
	scene.Add(std::move(changeTo));
	scene.Add(std::move(tileIcon));
	scene.Add(std::move(title));
}

void Game::LoadEndScreen(SceneType sceneType)
{
	auto& soundSystem = dae::ServiceLocator::GetSoundSystem();
	auto& input = dae::InputManager::GetInstance();
	auto& sceneManager = dae::SceneManager::GetInstance();

	glm::ivec2 windowSize = dae::Minigin::m_WindowSize;

	std::string sceneName = "EndScreen";
	auto& scene = sceneManager.CreateScene(sceneName);
	sceneManager.SetCurrentScene(sceneName);

	soundSystem.LoadSound("Change Selection.wav", dae::Sounds::ChangeSelection);

	auto font = dae::ServiceLocator::GetResourceManager().LoadFont("Monocraft.ttf", 40);
	glm::vec4 textColor{ 250,190,80,255 };

	auto endTitle = std::make_unique<dae::GameObject>();
	endTitle->AddComponent<dae::GraphicsComponent>("GameEnd Title.png");
	endTitle->AddComponent<dae::SpritesheetComponent>(1, 2);
	endTitle->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(0, (sceneType == SceneType::Win ? 0 : 1));
	endTitle->SetPosition((windowSize.x - endTitle->GetComponent<dae::GraphicsComponent>()->GetTextureSize().x) / 2.f, 50.f);
	scene.Add(std::move(endTitle));

	auto score = std::make_unique<dae::GameObject>();
	score->AddComponent<dae::GraphicsComponent>();
	score->AddComponent<dae::TextComponent>(font, "Score: " + std::to_string(Game::GetInstance().GetSavedScore()));
	score->GetComponent<dae::TextComponent>()->SetColor(textColor);
	score->SetPosition((windowSize.x - score->GetComponent<dae::GraphicsComponent>()->GetTextureSize().x) / 2.f, windowSize.y / 2.f + 50.f);
	scene.Add(std::move(score));

	auto highscoreName = std::make_unique<dae::GameObject>();
	highscoreName->AddComponent<dae::GraphicsComponent>();
	highscoreName->AddComponent<dae::TextComponent>(font, "AAA");
	highscoreName->GetComponent<dae::TextComponent>()->SetColor(textColor);
	highscoreName->SetPosition((windowSize.x - highscoreName->GetComponent<dae::GraphicsComponent>()->GetTextureSize().x) / 2.f, windowSize.y / 2.f + 100.f);

	auto highscoreSelection = std::make_unique<dae::GameObject>();
	highscoreSelection->AddComponent<HighScoreComponent>(scene.Add(std::move(highscoreName)));

	auto nextCharCommand   = std::make_shared<ChangeNameCommand>(highscoreSelection.get(), glm::ivec2{ 0, 1 });
	auto prevCharCommand   = std::make_shared<ChangeNameCommand>(highscoreSelection.get(), glm::ivec2{ 0,-1 });
	auto nextLetterCommand = std::make_shared<ChangeNameCommand>(highscoreSelection.get(), glm::ivec2{ 1, 0 });
	auto prevLetterCommand = std::make_shared<ChangeNameCommand>(highscoreSelection.get(), glm::ivec2{-1, 0 });
	auto saveHighscoreCommand = std::make_shared<SaveHighscoreCommand>(highscoreSelection.get());

	input.BindCommand(prevCharCommand,   SDL_SCANCODE_W, dae::InputType::Pressed);
	input.BindCommand(nextCharCommand,   SDL_SCANCODE_S, dae::InputType::Pressed);
	input.BindCommand(prevLetterCommand, SDL_SCANCODE_A, dae::InputType::Pressed);
	input.BindCommand(nextLetterCommand, SDL_SCANCODE_D, dae::InputType::Pressed);
	input.BindCommand(prevCharCommand,   SDL_SCANCODE_UP,	 dae::InputType::Pressed);
	input.BindCommand(nextCharCommand,   SDL_SCANCODE_DOWN,	 dae::InputType::Pressed);
	input.BindCommand(prevLetterCommand, SDL_SCANCODE_LEFT,	 dae::InputType::Pressed);
	input.BindCommand(nextLetterCommand, SDL_SCANCODE_RIGHT, dae::InputType::Pressed);
	input.BindCommand(prevCharCommand,   dae::ControllerButton::DPAD_UP,	dae::InputType::Pressed, 0);
	input.BindCommand(nextCharCommand,   dae::ControllerButton::DPAD_DOWN,	dae::InputType::Pressed, 0);
	input.BindCommand(prevLetterCommand, dae::ControllerButton::DPAD_LEFT,	dae::InputType::Pressed, 0);
	input.BindCommand(nextLetterCommand, dae::ControllerButton::DPAD_RIGHT,	dae::InputType::Pressed, 0);
	input.BindCommand(prevCharCommand,   dae::ControllerButton::DPAD_UP,	dae::InputType::Pressed, 1);
	input.BindCommand(nextCharCommand,   dae::ControllerButton::DPAD_DOWN,	dae::InputType::Pressed, 1);
	input.BindCommand(prevLetterCommand, dae::ControllerButton::DPAD_LEFT,	dae::InputType::Pressed, 1);
	input.BindCommand(nextLetterCommand, dae::ControllerButton::DPAD_RIGHT,	dae::InputType::Pressed, 1);
	input.BindCommand(saveHighscoreCommand, SDL_SCANCODE_RETURN,	  dae::InputType::Released);
	input.BindCommand(saveHighscoreCommand, dae::ControllerButton::A, dae::InputType::Released, 0);
	input.BindCommand(saveHighscoreCommand, dae::ControllerButton::A, dae::InputType::Released, 1);

	scene.Add(std::move(highscoreSelection));
}

void Game::LoadHighscoreScreen()
{
	auto& input = dae::InputManager::GetInstance();
	auto& sceneManager = dae::SceneManager::GetInstance();

	glm::ivec2 windowSize = dae::Minigin::m_WindowSize;

	std::string sceneName = "HighScores";
	auto& scene = sceneManager.CreateScene(sceneName);
	sceneManager.SetCurrentScene(sceneName);

	auto font = dae::ServiceLocator::GetResourceManager().LoadFont("Monocraft.ttf", 48);
	glm::vec4 textColor{ 255, 54, 11,255 };
	
	auto returnToMenuCommand = std::make_shared<ReturnToMenuCommand>();
	input.BindCommand(returnToMenuCommand, SDL_SCANCODE_RETURN, dae::InputType::Released);
	input.BindCommand(returnToMenuCommand, dae::ControllerButton::A, dae::InputType::Released, 0);
	input.BindCommand(returnToMenuCommand, dae::ControllerButton::A, dae::InputType::Released, 1);

	auto title = std::make_unique<dae::GameObject>();
	title->AddComponent<dae::GraphicsComponent>();
	title->AddComponent<dae::TextComponent>(font, "Highscores");
	title->GetComponent<dae::TextComponent>()->SetColor(textColor);
	title->SetPosition((windowSize.x - title->GetComponent<dae::GraphicsComponent>()->GetTextureSize().x) / 2.f, 20.f);
	scene.Add(std::move(title));

	font = dae::ServiceLocator::GetResourceManager().LoadFont("Monocraft.ttf", 24);
	textColor = glm::vec4{ 250,190,80,255 };

	auto vHighscoreData = GetHighscoreData();

	for (int i{}; i < static_cast<int>(vHighscoreData.size()); ++i)
	{
		const auto& [name, score] = vHighscoreData[i];

		std::string highscoreText = std::format("{}{}) {} {}", (i + 1 < 10) ? " " : "", i + 1, name, score);
		
		auto highscore = std::make_unique<dae::GameObject>();
		highscore->AddComponent<dae::GraphicsComponent>();
		highscore->AddComponent<dae::TextComponent>(font, highscoreText);
		highscore->GetComponent<dae::TextComponent>()->SetColor(textColor);
		glm::vec3 pos{ 0.f , (i % 10) * 30.f + 110.f, 0.f };
		if (i < 10)
			pos.x = 80.f;
		else
			pos.x = windowSize.x - 280.f;

		highscore->SetPosition(pos);
		scene.Add(std::move(highscore));
	}

	font = dae::ServiceLocator::GetResourceManager().LoadFont("Monocraft.ttf", 16);

	auto returnToMenu = std::make_unique<dae::GameObject>();
	returnToMenu->AddComponent<dae::GraphicsComponent>();
	returnToMenu->AddComponent<dae::TextComponent>(font, "Press enter (A for controller) to return to menu");
	returnToMenu->GetComponent<dae::TextComponent>()->SetColor(textColor);
	glm::ivec2 textSize = returnToMenu->GetComponent<dae::GraphicsComponent>()->GetTextureSize();
	returnToMenu->SetPosition((windowSize.x - textSize.x) / 2.f, windowSize.y - textSize.y - 20.f);
	scene.Add(std::move(returnToMenu));
}

std::vector<std::pair<std::string, int>> Game::GetHighscoreData() const
{
	json jsonArray = utils::ReadJson("../Data/Highscore.bin");

	std::vector<std::pair<std::string, int>> vHighscores(20, { "   ", 0 });

	size_t count = 0;
	for (const auto& element : jsonArray) 
	{
		if (count >= vHighscores.size())
			break;

		std::string name = element["name"].get<std::string>();
		int score = element["score"].get<int>();
		vHighscores[count++] = { name, score };
	}

	return vHighscores;
}

namespace glm 
{
	void from_json(const json& j, glm::ivec2& vec)
	{
		j.at("x").get_to(vec.x);
		j.at("y").get_to(vec.y);
	}
}

std::vector<SpawnData> Game::GetSpawnData() const
{
	std::string sceneName = dae::SceneManager::GetInstance().GetCurrentSceneName();

	json jsonArray = utils::ReadJson("../Data/" + sceneName + ".bin");

	std::vector<SpawnData> vSpawnData;
	for (const auto& item : jsonArray)
	{
		SpawnData data;
		data.minDelay = item["minSpawnDelay"];
		data.maxDelay = item["maxSpawnDelay"];
		data.vSpawnPositions = item["spawnPositions"].get<std::vector<glm::ivec2>>();
		vSpawnData.push_back(data);
	}

	return vSpawnData;
}


