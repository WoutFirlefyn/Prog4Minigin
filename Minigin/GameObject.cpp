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

dae::Transform dae::GameObject::GetTransform() const
{
	return m_Transform;
}

void dae::GameObject::MarkAsDestroyed()
{
	m_IsDestroyed = true;
}

bool dae::GameObject::IsDestroyed() const
{
	return m_IsDestroyed;
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}
