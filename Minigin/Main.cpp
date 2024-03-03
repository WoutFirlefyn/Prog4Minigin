#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include "GraphicsComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RotateComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::GraphicsComponent>(go.get(), "background.tga");
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::GraphicsComponent>(go.get(), "logo.tga");
	go->SetPosition(216, 180);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto pTitle = std::make_shared<dae::GameObject>();
	pTitle->AddComponent<dae::TextComponent>(pTitle.get(), "Programming 4 Assignment", font);
	pTitle->SetPosition(80, 20);
	scene.Add(pTitle);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextComponent>(go.get(), "000.0 FPS", font);
	go->AddComponent<dae::FPSComponent>(go.get());
	go->SetPosition(0, 50);
	go->SetParent(pTitle.get(), false);
	scene.Add(go);

	auto qbert1 = std::make_shared<dae::GameObject>();
	qbert1->AddComponent<dae::GraphicsComponent>(qbert1.get(), "Qbert-P1.tga");
	qbert1->AddComponent<dae::RotateComponent>(qbert1.get(), 6.f, 25.f);
	qbert1->SetPosition(100, 400);
	scene.Add(qbert1);

	auto qbert2 = std::make_shared<dae::GameObject>();
	qbert2->AddComponent<dae::GraphicsComponent>(qbert2.get(), "Qbert-P2.tga");
	qbert2->AddComponent<dae::RotateComponent>(qbert2.get(), -4.f, 25.f);
	qbert2->SetPosition(100, 400);
	qbert2->SetParent(qbert1.get());
	scene.Add(qbert2);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}