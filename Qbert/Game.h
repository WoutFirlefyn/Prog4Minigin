#pragma once
#include <Singleton.h>

enum class SceneType
{
	Solo = 0,
	Coop = 1,
	Versus = 2,
	Highscore = 3,
	MainMenu = 4,
	GameOver = 5,
	Win = 6
};

class Game final : public dae::Singleton<Game>
{
public:
	~Game() = default;

	Game(const Game&) = delete;
	Game(Game&&) noexcept = delete;
	Game& operator= (const Game&) = delete;
	Game& operator= (Game&&) noexcept = delete;

	void SetScene(SceneType scene);
	SceneType GetCurrentSceneType() const { return m_CurrentSceneType; }
	void SaveScore(int score) { m_SavedScore = score; }
	int GetSavedScore() const { return m_SavedScore; }
private:
	friend class dae::Singleton<Game>;
	Game() = default;

	void LoadMainMenu();
	void LoadLevel(SceneType sceneType);
	void LoadEndScreen(SceneType sceneType);
	void LoadHighscoreScreen();
	std::vector<std::pair<std::string, int>> GetHighscoreData() const;

	SceneType m_CurrentSceneType{};

	int m_SavedScore{ 0 };
};

