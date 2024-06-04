#pragma once
#include "GameObject.h"

namespace dae
{
	class BaseComponent
	{
	public:
		virtual ~BaseComponent() = default;

		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) noexcept = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other)	noexcept = delete;

		virtual void Init() {};
		virtual void Render() const {};
		virtual void Update() {};
		virtual void FixedUpdate() {};
		virtual void LateUpdate() {};
		virtual void RenderGUI() {};

	protected:
		BaseComponent(GameObject* pGameObject) : m_pGameObject{ pGameObject } {}
		GameObject* GetGameObject() const { return m_pGameObject; }
	private:
		GameObject* m_pGameObject;
	};
};

 
