#include "HighScoreComponent.h"
#include "TextComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"
#include "Game.h"
#include "Utils.h"

HighScoreComponent::HighScoreComponent(dae::GameObject* pGameObject, dae::GameObject* pNameObject)
	: BaseComponent(pGameObject)
	, m_pNameObject{ pNameObject }
{
	if (m_pNameObject->HasComponent<dae::TextComponent>())
		m_pNameObject->GetComponent<dae::TextComponent>()->SetText(m_Name);
}

void HighScoreComponent::ChangeLetter(const glm::ivec2& offset)
{
	if (offset.x != 0)
	{
		if (m_SelectedLetter == 0 && offset.x < 0)
			m_SelectedLetter = m_Name.size();

		m_SelectedLetter = (m_SelectedLetter + offset.x) % m_Name.size();
	}
	if (offset.y != 0)
	{
		m_Name[m_SelectedLetter] = 'A' + (static_cast<int>(m_Name[m_SelectedLetter] - 'A') + offset.y) % (static_cast<int>('Z' - 'A') + 1);

		if (m_Name[m_SelectedLetter] < 'A')
			m_Name[m_SelectedLetter] = 'Z';
	}

	if (!m_pNameObject->HasComponent<dae::TextComponent>())
	{
		assert(false);
		return;
	}

	dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::ChangeSelection);
	m_pNameObject->GetComponent<dae::TextComponent>()->SetText(m_Name);
}

void HighScoreComponent::SaveHighscore()
{
    dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::ChangeSelection);
    const std::string filename = "../Data/Highscore.bin";

    nlohmann::json jsonArray = utils::ReadJson(filename);

    nlohmann::json jsonData;
    jsonData["name"] = m_Name;
    jsonData["score"] = Game::GetInstance().GetSavedScore();

    jsonArray.push_back(jsonData);

    std::sort(jsonArray.begin(), jsonArray.end(), [](const nlohmann::json& a, const nlohmann::json& b)
        {
            return a["score"].get<int>() > b["score"].get<int>();
        });

    if (jsonArray.size() > 20)
        jsonArray.erase(jsonArray.begin() + 20, jsonArray.end());

	utils::WriteJson(filename, jsonArray);

    Game::GetInstance().SaveScore(0);
    Game::GetInstance().SetScene(SceneType::MainMenu);
}
