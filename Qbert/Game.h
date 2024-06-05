#pragma once
#include <Singleton.h>

enum class SceneType
{
	Solo = 0,
	Coop = 1,
	Versus = 2,
	MainMenu,
	Highscore
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
private:
	friend class dae::Singleton<Game>;
	Game() = default;

	void LoadMainMenu();
	void LoadSolo();
	void LoadCoop();
	void LoadVersus();
};

