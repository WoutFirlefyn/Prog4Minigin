#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Init()
{
	for (auto& pScene : m_vScenes)
		pScene->Init();
}

void dae::SceneManager::Update()
{
	for(auto& pScene : m_vScenes)
		pScene->Update();
}

void dae::SceneManager::FixedUpdate()
{
	for(auto& pScene : m_vScenes)
		pScene->FixedUpdate();
}

void dae::SceneManager::LateUpdate()
{
	for (auto& pScene : m_vScenes)
		pScene->LateUpdate();
}

void dae::SceneManager::Render() const
{
	for (const auto& pScene : m_vScenes)
		pScene->Render();
}

void dae::SceneManager::RenderGUI()
{
	for (auto& pScene : m_vScenes)
		pScene->RenderGUI();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_vScenes.push_back(scene);
	return *scene;
}
