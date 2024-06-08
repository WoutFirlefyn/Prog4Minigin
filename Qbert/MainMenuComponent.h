#pragma once
#include "BaseComponent.h"

enum class SceneType;

class MainMenuComponent : public dae::BaseComponent
{
public:
	MainMenuComponent(dae::GameObject* pGameObject, const std::vector<dae::GameObject*>& vText, dae::GameObject* pArrow);
	virtual ~MainMenuComponent() override = default;

	MainMenuComponent(const MainMenuComponent& other) = delete;
	MainMenuComponent(MainMenuComponent&& other) noexcept = delete;
	MainMenuComponent& operator=(const MainMenuComponent& other) = delete;
	MainMenuComponent& operator=(MainMenuComponent&& other)	noexcept = delete;

	void ChangeGamemode(int offset);
	void SelectGamemode();
private:
	std::vector<dae::GameObject*> m_vModes;
	dae::GameObject* m_pArrow{ nullptr };
	SceneType m_SelectedScene{};
};