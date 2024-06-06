#pragma once
#include <BaseComponent.h>
#include <Observer.h>
class LevelManagerComponent;
enum class Character;
class TitleComponent final : public dae::BaseComponent, public dae::Observer<bool>
{
public:
	TitleComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent);
	virtual ~TitleComponent() override = default;

	TitleComponent(const TitleComponent& other) = delete;
	TitleComponent(TitleComponent&& other) noexcept = delete;
	TitleComponent& operator=(const TitleComponent& other) = delete;
	TitleComponent& operator=(TitleComponent&& other) noexcept = delete;

	virtual void Init() override;
	//virtual void Update() override;

	virtual void Notify(bool nextLevel) override;
	virtual void SubjectDestroyed(dae::Subject<bool>* pSubject) override;
private:
	LevelManagerComponent* m_pLevelManagerComponent{ nullptr };
	dae::Subject<bool>* m_pNewRoundStartedSubject{ nullptr };
};

