#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include <memory>
#include "Singleton.h"

//-----------------------------------------------------
// Controller Class									
//-----------------------------------------------------
namespace dae
{
	class Controller final : public Singleton<Controller>
	{
	public:
		Controller();

		~Controller();

		Controller(const Controller& other) = delete;
		Controller(Controller&& other) noexcept = delete;
		Controller& operator=(const Controller& other) = delete;
		Controller& operator=(Controller&& other) noexcept = delete;

		void ProcessInput();
		
		bool IsPressedThisFrame(unsigned int button) const;
		bool IsUpThisFrame(unsigned int button) const;
		bool IsDown(unsigned int button) const;
		bool IsThumbsNotInDeadZone() const;

	private:
		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		class ControllerImpl;
		std::unique_ptr<ControllerImpl> m_pControllerImpl;
	};
}

 
