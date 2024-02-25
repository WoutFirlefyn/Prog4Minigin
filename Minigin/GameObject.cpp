#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <algorithm>

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime) 
{
	for (std::shared_ptr<BaseComponent> pComponents : m_vComponents)
	{
		pComponents->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	for (auto& pComponents : m_vComponents)
	{
		pComponents->Render();
	}
}

dae::Transform dae::GameObject::GetPosition() const
{
	return m_transform;
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
