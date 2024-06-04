#pragma once
#include "BaseComponent.h"

enum class Gamemode
{
	Solo = 0,
	Coop = 1,
	Versus = 2
};

class MainMenuComponent : public dae::BaseComponent
{
public:
	MainMenuComponent(dae::GameObject* pGameObject);
	virtual ~MainMenuComponent() override = default;

	MainMenuComponent(const MainMenuComponent& other) = delete;
	MainMenuComponent(MainMenuComponent&& other) noexcept = delete;
	MainMenuComponent& operator=(const MainMenuComponent& other) = delete;
	MainMenuComponent& operator=(MainMenuComponent&& other)	noexcept = delete;

	//virtual void Init() override;
	//virtual void Update() override;
	void ChangeGamemode(int);
	void SelectGamemode();
private:
	std::vector<dae::GameObject*> m_vModes;
	dae::GameObject* m_pArrow{ nullptr };
	Gamemode m_CurrentGamemode{};
};