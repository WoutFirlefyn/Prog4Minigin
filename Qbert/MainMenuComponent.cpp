#include "MainMenuComponent.h"
#include "GraphicsComponent.h"
#include "ServiceLocator.h"
#include "Scene.h"

MainMenuComponent::MainMenuComponent(dae::GameObject* pGameObject) 
	: BaseComponent(pGameObject)
{
}

void MainMenuComponent::ChangeGamemode(int delta)
{
    int newModeIndex = (static_cast<int>(m_CurrentGamemode) + delta) % 3;

    if (newModeIndex < 0)
        newModeIndex += 3;

    m_CurrentGamemode = static_cast<Gamemode>(newModeIndex);
}

void MainMenuComponent::SelectGamemode()
{
    auto& sceneManager = dae::SceneManager::GetInstance();
    switch (m_CurrentGamemode)
    {
    case Gamemode::Solo:
        sceneManager.SetCurrentScene("Solo");
        break;
    case Gamemode::Coop:
        sceneManager.SetCurrentScene("Coop");
        break;
    case Gamemode::Versus:
        sceneManager.SetCurrentScene("Versus");
        break;
    }
}
