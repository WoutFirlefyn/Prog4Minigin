#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <algorithm>

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime) 
{ 
	(void)deltaTime; 
	//for (std::shared_ptr<BaseComponent> pComponents : m_vComponents)
	//{
	//	pComponents->Update(this);
	//}
}

void dae::GameObject::Render() const
{
	//m_vComponents[0]->Update(this);
	//const auto& pos = m_transform.GetPosition();
	//Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}

void dae::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

dae::Transform dae::GameObject::GetPosition() const
{
	return m_transform;
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

void dae::GameObject::AddComponent(std::shared_ptr<BaseComponent> pComponent)
{
	m_vComponents.push_back(pComponent);
}

void dae::GameObject::RemoveComponent(std::shared_ptr<BaseComponent> pComponent)
{

}
