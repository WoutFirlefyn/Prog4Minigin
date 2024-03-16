#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include <Xinput.h>
#include "Singleton.h"

//-----------------------------------------------------
// Controller Class									
//-----------------------------------------------------
namespace dae
{
	class Controller final : public Singleton<Controller>
	{
	public:
		Controller() = default;
		~Controller() = default;

		Controller(const Controller& other) = delete;
		Controller(Controller&& other) noexcept = delete;
		Controller& operator=(const Controller& other) = delete;
		Controller& operator=(Controller&& other)	noexcept = delete;

		void ProcessInput();
		
		bool IsPressedThisFrame(unsigned int button) const;
		bool IsUpThisFrame(unsigned int button) const;
		bool IsDown(unsigned int button) const;
		bool IsThumbsNotInDeadZone() const;

	private:
		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		DWORD m_ControllerIndex{};
		int m_ButtonsPressedThisFrame{};
		int m_ButtonsReleasedThisFrame{};

		XINPUT_STATE m_PreviousState;
		XINPUT_STATE m_CurrentState;
		float m_DeadzonePercentage{ 20.f };
	};
}

 
