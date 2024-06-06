#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Init()
{
	m_Scenes.at(m_CurrentScene)->Init();
}

void dae::SceneManager::Update()
{
	m_Scenes.at(m_CurrentScene)->Update();
}

void dae::SceneManager::FixedUpdate()
{
	m_Scenes.at(m_CurrentScene)->FixedUpdate();
}

void dae::SceneManager::LateUpdate()
{
	m_Scenes.at(m_CurrentScene)->LateUpdate();
}

void dae::SceneManager::Render() const
{
	m_Scenes.at(m_CurrentScene)->Render();
}

void dae::SceneManager::RenderGUI()
{
	m_Scenes.at(m_CurrentScene)->RenderGUI();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	if (m_Scenes.contains(name))
		std::cout << "Overwriting existing scene\n";
	m_Scenes[name] = std::unique_ptr<Scene>(new Scene());
	return *m_Scenes[name];
}

dae::Scene& dae::SceneManager::GetScene(const std::string& name) const
{
	auto scenePairIt = m_Scenes.find(name);
	if (scenePairIt == m_Scenes.end())
		assert(false && "That scene does not exist");
	return *scenePairIt->second;
}

void dae::SceneManager::SetCurrentScene(const std::string& name)
{
	if (!m_Scenes.contains(name))
	{
		assert(false && "That scene does not exist");
		return;
	}
	m_CurrentScene = name;
}

dae::Scene& dae::SceneManager::GetCurrentScene() const
{
	auto scenePairIt = m_Scenes.find(m_CurrentScene);
	if (scenePairIt == m_Scenes.end())
		assert(false && "No active scene");
	return *scenePairIt->second;
}
