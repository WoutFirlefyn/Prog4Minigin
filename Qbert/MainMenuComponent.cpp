#include "MainMenuComponent.h"
#include "GraphicsComponent.h"
#include "ServiceLocator.h"
#include "Scene.h"
#include "Sounds.h"
#include "ServiceLocator.h"

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

    m_pArrow->SetParent(GetGameObject());
    m_pArrow->SetScale(2,2);
    glm::ivec2 arrowSize = m_pArrow->GetComponent<dae::GraphicsComponent>()->GetTextureSize() * 2;
    glm::ivec2 textSize = m_vModes[0]->GetComponent<dae::GraphicsComponent>()->GetTextureSize();
    m_pArrow->SetPosition(m_vModes[0]->GetLocalPosition() - glm::vec3{ 20, textSize.y / 2.f, 0 } + glm::vec3{ 0, arrowSize.y / 1.25f, 0 });
}

void MainMenuComponent::ChangeGamemode(int offset)
{
    int newModeIndex = (static_cast<int>(m_CurrentGamemode) + offset) % 3;

    if (newModeIndex < 0)
        newModeIndex += 3;

    m_CurrentGamemode = static_cast<Gamemode>(newModeIndex);
    glm::ivec2 arrowSize = m_pArrow->GetComponent<dae::GraphicsComponent>()->GetTextureSize() * 2;
    glm::ivec2 textSize = m_vModes[newModeIndex]->GetComponent<dae::GraphicsComponent>()->GetTextureSize();
    m_pArrow->SetPosition(m_vModes[newModeIndex]->GetLocalPosition() - glm::vec3{ 20, textSize.y / 2.f, 0 } + glm::vec3{ 0, arrowSize.y / 1.25f, 0 });

    dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::ChangeSelection, 0.5f);
}

void MainMenuComponent::SelectGamemode()
{
    dae::SceneManager::GetInstance().SetCurrentScene("Level");
}
