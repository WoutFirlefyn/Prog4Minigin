#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <algorithm>

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update() 
{
	for (std::shared_ptr<BaseComponent> pComponents : m_vComponents)
	{
		pComponents->Update();
	}
}
void dae::GameObject::FixedUpdate() 
{

}

void dae::GameObject::Render() const
{
	for (auto& pComponents : m_vComponents)
	{
		pComponents->Render();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

void dae::GameObject::SetParent(GameObject* pParent)
{
	if (pParent == nullptr)
		return;

	m_pParent = pParent;
}
