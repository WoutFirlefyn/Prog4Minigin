#pragma once
#include <BaseComponent.h>

class HighScoreComponent : public dae::BaseComponent
{
public:
	HighScoreComponent(dae::GameObject* pGameObject, dae::GameObject* pNameObject);
	virtual ~HighScoreComponent() override = default;

	HighScoreComponent(const HighScoreComponent& other) = delete;
	HighScoreComponent(HighScoreComponent&& other) noexcept = delete;
	HighScoreComponent& operator=(const HighScoreComponent& other) = delete;
	HighScoreComponent& operator=(HighScoreComponent&& other)	noexcept = delete;

	void ChangeLetter(const glm::ivec2& offset);
private:
	dae::GameObject* m_pNameObject;
	std::string m_Letters = { "AAA" };
	size_t m_SelectedLetter{ 0 };
};

