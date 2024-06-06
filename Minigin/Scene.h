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
		explicit Scene();
		void CheckForDestroyedObjects();
		void AddNewObjects();

		std::vector<std::unique_ptr<GameObject>> m_vObjects{};
		std::vector<std::unique_ptr<GameObject>> m_vNewObjects{};

		bool m_IsInitialized{ false };
	};
}
