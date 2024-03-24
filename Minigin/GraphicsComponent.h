#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include <memory>
#include <string>
#include "BaseComponent.h"

namespace dae
{
	class GameObject;
	class Texture2D;
	//-----------------------------------------------------
	// GraphicsComponent Class									
	//-----------------------------------------------------
	class GraphicsComponent final : public BaseComponent
	{
	public:
		GraphicsComponent(GameObject* pGameObject, const std::string& name);				// Constructor
		virtual ~GraphicsComponent() override = default;		// Destructor

		// -------------------------
		// Copy/move constructors and assignment operators
		// -------------------------    
		GraphicsComponent(const GraphicsComponent& other) = delete;
		GraphicsComponent(GraphicsComponent&& other) noexcept = delete;
		GraphicsComponent& operator=(const GraphicsComponent& other) = delete;
		GraphicsComponent& operator=(GraphicsComponent&& other)	noexcept = delete;

		//-------------------------------------------------
		// Member functions						
		//-------------------------------------------------
		virtual void Render() const override;
		void SetTexture(const std::string& filename);

	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------


		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		std::shared_ptr<Texture2D> m_pTexture{};


	};
}
 
