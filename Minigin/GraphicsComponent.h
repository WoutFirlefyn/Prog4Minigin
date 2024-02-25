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
	class GraphicsComponent : public BaseComponent
	{
	public:
		GraphicsComponent();				// Constructor
		virtual ~GraphicsComponent();		// Destructor

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
		void Render() const;
		virtual void Update(GameObject* pGameObject) override;
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
 
