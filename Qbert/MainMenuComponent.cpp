#include "MainMenuComponent.h"
#include "GraphicsComponent.h"
#include "ServiceLocator.h"
#include "Scene.h"

MainMenuComponent::MainMenuComponent(dae::GameObject* pGameObject, const std::vector<dae::GameObject*>& vText, dae::GameObject* pArrow)
	: BaseComponent(pGameObject)
    , m_vModes{ vText }
    , m_pArrow{ pArrow }
{
    for (size_t i{}; i < m_vModes.size(); ++i)
    {
        glm::ivec2 textSize{ m_vModes[i]->GetComponent<dae::GraphicsComponent>()->GetTextureSize() };

        m_vModes[i]->SetParent(GetGameObject());
        m_vModes[i]->SetPosition(m_vModes[i]->GetLocalPosition() + glm::vec3{ -textSize.x / 2.f, 1.5f * textSize.y * i, 0 });
    }

    //m_pArrow->SetPosition
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
