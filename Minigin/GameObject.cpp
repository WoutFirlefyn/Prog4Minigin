#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include <algorithm>
#include <string>

dae::GameObject::~GameObject() = default;

void dae::GameObject::Init()
{
	for (auto& pComponents : m_Components)
		pComponents.second->Init();
}

void dae::GameObject::Update()
{
	for (auto& pComponents : m_Components)
		pComponents.second->Update();
}
void dae::GameObject::FixedUpdate() 
{
	for (auto& pComponents : m_Components)
		pComponents.second->FixedUpdate();
}

void dae::GameObject::LateUpdate()
{
	for (auto& pComponents : m_Components)
		pComponents.second->LateUpdate();
}

void dae::GameObject::Render() const
{
	for (auto& pComponents : m_Components)
		pComponents.second->Render();
}

void dae::GameObject::RenderGUI()
{
	for (auto& pComponents : m_Components)
		pComponents.second->RenderGUI();
}

void dae::GameObject::SetPosition(float x, float y)
{
	SetPosition(glm::vec3{ x,y,0.f });
}

void dae::GameObject::SetPosition(const glm::vec3& pos)
{
	m_LocalTransform.SetPosition(pos);
	SetPositionDirty();
}

void dae::GameObject::SetScale(float x, float y)
{
	SetScale(glm::vec3{ x,y,0.f });
}

void dae::GameObject::SetScale(const glm::vec3& scale)
{
	m_LocalTransform.SetScale(scale);
	SetPositionDirty();
}

void dae::GameObject::SetLocalTransform(const dae::Transform& transform)
{
	m_LocalTransform = transform;
	SetPositionDirty();
}

const dae::Transform& dae::GameObject::GetWorldTransform()
{
	if (m_PositionIsDirty)
		UpdateWorldTransform();
	return m_WorldTransform;
}

void dae::GameObject::UpdateWorldTransform()
{
	if (m_PositionIsDirty)
	{
		if (m_pParent == nullptr)
			m_WorldTransform = m_LocalTransform;
		else
			m_WorldTransform = m_pParent->GetWorldTransform() + m_LocalTransform;
	}
	m_PositionIsDirty = false;
}

void dae::GameObject::SetParent(GameObject* pParent, bool keepWorldTransform)
{
	if (pParent == this || IsChild(pParent) || pParent == m_pParent)
		return;

	if (pParent)
	{
		if(keepWorldTransform)
			SetLocalTransform(GetWorldTransform() - pParent->GetWorldTransform());
	}
	else
		SetLocalTransform(GetWorldTransform());
	
	SetPositionDirty();

	if (m_pParent)
		m_pParent->RemoveChild(this);

	m_pParent = pParent;

	if (m_pParent)
		m_pParent->AddChild(this);
}

void dae::GameObject::AddChild(GameObject* pChild)
{
	if (pChild == nullptr || pChild->IsChild(this) || pChild == m_pParent)
		return;

	m_vChildren.emplace_back(pChild);
}

void dae::GameObject::RemoveChild(GameObject* pChild)
{
	if (pChild == nullptr || !pChild->IsChild(this))
		return;

	m_vChildren.erase(std::remove(m_vChildren.begin(), m_vChildren.end(), pChild), m_vChildren.end());
}

bool dae::GameObject::IsChild(GameObject* pGameObject) const
{
	return std::ranges::find(m_vChildren, pGameObject) != m_vChildren.end();
}

void dae::GameObject::SetPositionDirty()
{
	m_PositionIsDirty = true;

	for (auto child : m_vChildren)
		child->SetPositionDirty();
}


