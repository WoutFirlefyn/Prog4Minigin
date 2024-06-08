#include "HighScoreComponent.h"
#include "TextComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"
#include "Game.h"
#include <json.hpp>
#include <fstream>

using json = nlohmann::json;

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
		m_SelectedLetter = (m_SelectedLetter + offset.x) % m_Name.size();

		if (m_SelectedLetter < 0)
			m_SelectedLetter = m_Name.size() - 1;
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
    const std::string filename = "Highscores.json";

    json jsonArray = json::array();

    std::ifstream inFile(filename);
    if (!inFile.is_open())
    {
        std::cerr << "Failed to open " << filename << "\n";
        return;
    }

    try 
    {
        inFile >> jsonArray;
    }
    catch (const json::parse_error& e) 
    {
        if (e.id != 101) // if json is empty, don't return
        {
            std::cerr << "Parse error: " << e.what() << std::endl;
            return;
        }
    }
    inFile.close();

    json jsonData;
    jsonData["name"] = m_Name;
    jsonData["score"] = Game::GetInstance().GetSavedScore();

    jsonArray.push_back(jsonData);

    std::sort(jsonArray.begin(), jsonArray.end(), [](const json& a, const json& b) 
        {
            return a["score"].get<int>() > b["score"].get<int>();
        });

    if (jsonArray.size() > 20)
        jsonArray.erase(jsonArray.begin() + 20, jsonArray.end());

    std::ofstream outFile(filename);
    if (!outFile.is_open())
    {
        std::cerr << "Error opening file for writing\n";
        return;
    }

    outFile << jsonArray.dump(4); // 4 is the indentation level for pretty printing
    outFile.close();

    Game::GetInstance().SaveScore(0);
    Game::GetInstance().SetScene(SceneType::MainMenu);
}
