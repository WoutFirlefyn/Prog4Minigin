#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include <glm/glm.hpp>

enum class DiskState
{
	Start,
	Stop
};

struct Disk
{
	dae::GameObject* pGameObject{ nullptr };
	DiskState state{ DiskState::Start };
};

namespace dae
{
	class SpritesheetComponent;
}
enum class Character;
class LevelManagerComponent;
class DiskComponent final : public dae::BaseComponent, public dae::Observer<Disk, Character>, public dae::Observer<>
{
public:
	DiskComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent);
	virtual ~DiskComponent() override;			

	DiskComponent(const DiskComponent& other) = delete;
	DiskComponent(DiskComponent&& other) noexcept = delete;
	DiskComponent& operator=(const DiskComponent& other) = delete;
	DiskComponent& operator=(DiskComponent&& other)	noexcept = delete;

	virtual void Init() override;
	virtual void Update() override;

	virtual void Notify(Disk, Character) override;

	virtual void Notify() override;
	virtual void SubjectDestroyed(dae::Subject<>* pSubject) override;

	static std::unique_ptr<dae::Subject<Disk, Character>> DiskStateChanged;
private:
	dae::Subject<>* m_pNewRoundStarted{ nullptr };

	dae::SpritesheetComponent* m_pSpritesheetComponent{ nullptr };
	LevelManagerComponent* m_pLevelManagerComponent{ nullptr };
	glm::vec3 m_StartPos{};
	glm::vec3 m_EndPos{};
	Character m_Character{};
	const float m_TimeToReachTop{ 2.5f };
	float m_PlatformLerpValue{ 0.f };
	float m_AccumSec{ 0.f };
	float m_Fps{ 12.f };
};

 
