#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include <vector>
#include <memory>
#include "BaseComponent.h"

//-----------------------------------------------------
// LevelManagerComponent Class									
//-----------------------------------------------------
namespace dae
{
	class Scene;
	class TileComponent;
	class LevelManagerComponent final : public BaseComponent
	{
	public:
		LevelManagerComponent(GameObject* pGameObject, Scene& scene);				// Constructor
		~LevelManagerComponent() = default;				// Destructor

		// -------------------------
		// Copy/move constructors and assignment operators
		// -------------------------    
		LevelManagerComponent(const LevelManagerComponent& other)				 = delete;
		LevelManagerComponent(LevelManagerComponent&& other) noexcept			 = delete;
		LevelManagerComponent& operator=(const LevelManagerComponent& other)	 = delete;
		LevelManagerComponent& operator=(LevelManagerComponent&& other)	noexcept = delete;

		//-------------------------------------------------
		// Member functions						
		//-------------------------------------------------
		virtual void Init() override;
		virtual void Update() override;

	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------
		std::vector<TileComponent*> GetAdjacentTiles(const std::vector<std::vector<GameObject*>>& vTiles, size_t row, size_t col) const;

		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		//std::vector<std::vector<GameObject*>> m_vTiles;
		int m_LevelLength{ 7 };

	};
}

 
