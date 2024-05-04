#pragma once
#include <memory>
#include "SoundSystem.h"
#include "ResourceManager.h"

namespace dae
{
	class ServiceLocator final
	{
	public:
		static SoundSystem& GetSoundSystem() { return *m_pSoundSystemInstance; }
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& pSoundSystem) { m_pSoundSystemInstance = std::move(pSoundSystem); }
		static BaseResourceManager& GetResourceManager() { return *m_pResourceManagerInstance; }
		static void RegisterResourceManager(std::unique_ptr<BaseResourceManager>&& pResourceManager) { m_pResourceManagerInstance = std::move(pResourceManager); }
	private:
		static std::unique_ptr<SoundSystem> m_pSoundSystemInstance;
		static std::unique_ptr<BaseResourceManager> m_pResourceManagerInstance;
	};
}
