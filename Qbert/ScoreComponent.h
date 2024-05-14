#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	class TextComponent;
}
class QbertComponent;
class LevelManagerComponent;
enum class ScoreType;
class ScoreComponent final : public dae::BaseComponent, public dae::Observer<bool>
{
public:
	ScoreComponent(dae::GameObject* pGameObject);
	virtual ~ScoreComponent() override;

	ScoreComponent(const ScoreComponent& other) = delete;
	ScoreComponent(ScoreComponent&& other) noexcept = delete;
	ScoreComponent& operator=(const ScoreComponent& other) = delete;
	ScoreComponent& operator=(ScoreComponent&& other)	noexcept = delete;

	virtual void Init() override;

	void AddObserver(dae::Subject<bool>* pTileChangedSubject);
	virtual void Notify(bool roundFinished) override;
	virtual void SubjectDestroyed(dae::Subject<bool>* pSubject) override;

	int GetScore() const { return m_Score; }

private:
	void UpdateText();

	dae::Subject<bool>* m_pTileChangedSubject{ nullptr };
	dae::TextComponent* m_pTextComponent{ nullptr };
	int m_Score{ 0 };
};


 
