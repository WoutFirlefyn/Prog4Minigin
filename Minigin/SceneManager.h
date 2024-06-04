#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		Scene& GetScene(const std::string& name) const;
		void SetCurrentScene(const std::string& name);

		void Init();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;
		void RenderGUI();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		std::unordered_map<std::string, std::shared_ptr<Scene>> m_Scenes;
		std::string m_CurrentScene{};
	};
}
