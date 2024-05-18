#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace dae
{
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		GameObject* Add(std::unique_ptr<GameObject>&& object);
		void Remove(std::unique_ptr<GameObject>&& object);
		void RemoveAll();

		void Init();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;
		void RenderGUI();

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);
		void CheckForDestroyedObjects();

		std::string m_name;
		std::vector <std::unique_ptr<GameObject>> m_vObjects{};

		static unsigned int m_idCounter; 
	};

}
