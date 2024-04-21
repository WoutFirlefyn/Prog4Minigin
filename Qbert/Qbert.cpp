#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

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
#include "LevelManagerComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
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
	fps->AddComponent<dae::FPSComponent>();
	fps->SetPosition(10, 10);

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

	// Tiles
	auto manager = std::make_unique<dae::GameObject>();
	manager->AddComponent<dae::LevelManagerComponent>(qbert1->GetComponent<dae::QbertComponent>(), scene);

	qbert1->GetComponent<dae::QbertComponent>()->AddObserver(manager->GetComponent<dae::LevelManagerComponent>());

	// Qbert curse
	auto qbertCurse = std::make_unique<dae::GameObject>();
	qbertCurse->AddComponent<dae::GraphicsComponent>("Qbert Curses.png", false);
	qbertCurse->AddComponent<dae::QbertCurseComponent>(qbert1->GetComponent<dae::QbertComponent>());
	qbertCurse->SetParent(qbert1.get());
	qbertCurse->SetPosition(-18, -30);

	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	// Lives 1
	std::vector<std::unique_ptr<dae::GameObject>> vHearts{};

	for (size_t i{}; i < 3; ++i)
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
	scoreDisplay->AddComponent<dae::ScoreComponent>(qbert1->GetComponent<dae::QbertComponent>(), manager->GetComponent<dae::LevelManagerComponent>());
	scoreDisplay->SetPosition(10, 50);

	scene.Add(std::move(manager));
	scene.Add(std::move(fps));
	scene.Add(std::move(scoreDisplay));
	for (auto& heart : vHearts)
		scene.Add(std::move(heart));
	scene.Add(std::move(qbertCurse));
	scene.Add(std::move(qbert1));

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

int main(int, char* []) 
{
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}