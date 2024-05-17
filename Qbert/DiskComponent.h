#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include <glm/glm.hpp>

namespace dae
{
	class SpritesheetComponent;
}
enum class Character;
enum class MovementState;
enum class MovementDirection;
class QbertComponent;
class DiskComponent final : public dae::BaseComponent
{
public:
	DiskComponent(dae::GameObject* pGameObject, dae::GameObject* pTopTile);
	~DiskComponent() = default;			

	DiskComponent(const DiskComponent& other) = delete;
	DiskComponent(DiskComponent&& other) noexcept = delete;
	DiskComponent& operator=(const DiskComponent& other) = delete;
	DiskComponent& operator=(DiskComponent&& other)	noexcept = delete;

	virtual void Init() override;
	virtual void Update() override;

	void MoveCharacterHere(const std::pair<Character, dae::GameObject*>& character);

	inline static const float m_TimeToReachTop{ 2.5f };
private:
	dae::GameObject* m_pTopTile;
	std::pair<Character, dae::GameObject*> m_pCharacter;
	dae::SpritesheetComponent* m_pSpritesheetComponent;
	glm::vec3 m_StartPos{};
	float m_PlatformLerpValue{ 0.f };
	float m_AccumSec{};
	float m_Fps{ 12.f };
};

 
