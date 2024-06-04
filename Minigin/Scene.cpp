#include "Scene.h"
#include "GameObject.h"
#include <algorithm>

//unsigned int Scene::m_idCounter = 0;

dae::Scene::Scene() = default;

dae::Scene::~Scene() = default;

dae::GameObject* dae::Scene::Add(std::unique_ptr<GameObject>&& object)
{
	m_vObjects.emplace_back(std::move(object));
	if (m_IsInitialized)
		m_vObjects.back()->Init();
	return m_vObjects.back().get();
}

void dae::Scene::Remove(std::unique_ptr<GameObject>&& object)
{
	m_vObjects.erase(std::remove(m_vObjects.begin(), m_vObjects.end(), object), m_vObjects.end());
}

void dae::Scene::RemoveAll()
{
	m_vObjects.clear();
}

void dae::Scene::Init()
{
	for (auto& pObject : m_vObjects)
		pObject->Init();

	m_IsInitialized = true;
}

void dae::Scene::Update()
{
	for(auto& pObject : m_vObjects)
		if (!pObject->IsDestroyed())
			pObject->Update();

	CheckForDestroyedObjects();
}

void dae::Scene::FixedUpdate()
{
	for(auto& pObject : m_vObjects)
		if (!pObject->IsDestroyed())
			pObject->FixedUpdate();

	CheckForDestroyedObjects();
}

void dae::Scene::LateUpdate()
{
	for (auto& pObject : m_vObjects)
		if (!pObject->IsDestroyed())
			pObject->LateUpdate();

	CheckForDestroyedObjects();
}

void dae::Scene::Render() const
{
	for (const auto& pObject : m_vObjects)
		pObject->Render();
}

void dae::Scene::RenderGUI()
{
	for (auto& pObject : m_vObjects)
		pObject->RenderGUI();
}

void dae::Scene::CheckForDestroyedObjects()
{
	m_vObjects.erase(std::remove_if(m_vObjects.begin(), m_vObjects.end(),
		[&](const auto& pObject)
		{
			return pObject->IsDestroyed();
		}
	), m_vObjects.end());
}