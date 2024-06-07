#pragma once
#include <Singleton.h>

enum class SceneType
{
	Solo = 0,
	Coop = 1,
	Versus = 2,
	MainMenu = 3,
	Highscore = 4
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
private:
	friend class dae::Singleton<Game>;
	Game() = default;

	void LoadMainMenu();
	void LoadLevel(SceneType sceneType);
	void LoadEndScreen();

	SceneType m_CurrentSceneType;
};

