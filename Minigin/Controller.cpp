//---------------------------
// Includes
//---------------------------
//#include <Xinput.h>
#include <SDL_syswm.h>
#include <cmath>
#include "Controller.h"

void dae::Controller::ProcessInput()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}

bool dae::Controller::IsPressedThisFrame(unsigned int button) const
{
	return m_ButtonsPressedThisFrame & button;
}

bool dae::Controller::IsUpThisFrame(unsigned int button) const
{
	return m_ButtonsReleasedThisFrame & button;
}

bool dae::Controller::IsDown(unsigned int button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}

bool dae::Controller::IsThumbsNotInDeadZone() const
{
	const SHORT thumbL{ m_CurrentState.Gamepad.sThumbLX };
	const SHORT thumbR{ m_CurrentState.Gamepad.sThumbRX };
	const float percentageThumbL{ std::abs(thumbL / static_cast<float>(SHRT_MIN)) };
	const float percentageThumbR{ std::abs(thumbR / static_cast<float>(SHRT_MAX)) };

	return (percentageThumbL > m_DeadzonePercentage) || (percentageThumbR > m_DeadzonePercentage);
}
