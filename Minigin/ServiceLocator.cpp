#include "ServiceLocator.h"

std::unique_ptr<dae::SoundSystem> dae::ServiceLocator::m_pSoundSystemInstance{ std::make_unique<dae::NullSoundSystem>() };
std::unique_ptr<dae::BaseResourceManager> dae::ServiceLocator::m_pResourceManagerInstance{ std::make_unique<dae::ResourceManager>() };