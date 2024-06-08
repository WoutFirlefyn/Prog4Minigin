#include "HighScoreComponent.h"
#include "TextComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"

HighScoreComponent::HighScoreComponent(dae::GameObject* pGameObject, dae::GameObject* pNameObject)
	: BaseComponent(pGameObject)
	, m_pNameObject{ pNameObject }
{
	m_pNameObject->SetParent(GetGameObject());
	if (m_pNameObject->HasComponent<dae::TextComponent>())
		m_pNameObject->GetComponent<dae::TextComponent>()->SetText(m_Letters);
}

void HighScoreComponent::ChangeLetter(const glm::ivec2& offset)
{
	if (offset.x != 0)
	{
		m_SelectedLetter = (m_SelectedLetter + offset.x) % m_Letters.size();

		if (m_SelectedLetter < 0)
			m_SelectedLetter = m_Letters.size() - 1;
	}
	if (offset.y != 0)
	{
		m_Letters[m_SelectedLetter] = 'A' + (static_cast<int>(m_Letters[m_SelectedLetter] - 'A') + offset.y) % (static_cast<int>('Z' - 'A') + 1);

		if (m_Letters[m_SelectedLetter] < 'A')
			m_Letters[m_SelectedLetter] = 'Z';
	}

	dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::ChangeSelection);

	if (!m_pNameObject->HasComponent<dae::TextComponent>())
	{
		assert(false);
		return;
	}

	m_pNameObject->GetComponent<dae::TextComponent>()->SetText(m_Letters);
}
