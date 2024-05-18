#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include <glm/glm.hpp>

namespace dae
{
	class SpritesheetComponent;
}
enum class Character;
class DiskComponent final : public dae::BaseComponent, public dae::Observer<dae::GameObject*, Character>
{
public:
	DiskComponent(dae::GameObject* pGameObject);
	virtual ~DiskComponent() override;			

	DiskComponent(const DiskComponent& other) = delete;
	DiskComponent(DiskComponent&& other) noexcept = delete;
	DiskComponent& operator=(const DiskComponent& other) = delete;
	DiskComponent& operator=(DiskComponent&& other)	noexcept = delete;

	virtual void Init() override;
	virtual void Update() override;

	virtual void Notify(dae::GameObject*, Character) override;

	std::pair<Character, dae::GameObject*> GetCharacter();
	void MoveCharacterHere(const std::pair<Character, dae::GameObject*>& character, dae::GameObject* pTopTile);

	static std::unique_ptr<dae::Subject<dae::GameObject*, Character>> DiskReachedTop;
private:
	std::pair<Character, dae::GameObject*> m_pCharacter{};
	dae::SpritesheetComponent* m_pSpritesheetComponent{ nullptr };
	glm::vec3 m_StartPos{};
	glm::vec3 m_EndPos{};
	const float m_TimeToReachTop{ 2.5f };
	float m_PlatformLerpValue{ 0.f };
	float m_AccumSec{ 0.f };
	float m_Fps{ 12.f };
};

 
