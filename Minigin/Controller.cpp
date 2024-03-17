//---------------------------
// Includes
//---------------------------
#include <windows.h>
#include <Xinput.h>
#include <SDL_syswm.h>
#include <cmath>
#include "Controller.h"
class dae::Controller::ControllerImpl
{
public:
	ControllerImpl() = default;
	~ControllerImpl() = default;

	ControllerImpl(const ControllerImpl& other) = delete;
	ControllerImpl(ControllerImpl&& other) noexcept = delete;
	ControllerImpl& operator=(const ControllerImpl& other) = delete;
	ControllerImpl& operator=(ControllerImpl&& other) noexcept = delete;

	void ProcessInput();

	bool IsPressedThisFrame(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsDown(unsigned int button) const;
	bool IsThumbsNotInDeadZone() const;
private:
	DWORD m_ControllerIndex{};
	int m_ButtonsPressedThisFrame{};
	int m_ButtonsReleasedThisFrame{};

	XINPUT_STATE m_PreviousState{};
	XINPUT_STATE m_CurrentState{};
	float m_DeadzonePercentage{ 20.f };
};

void dae::Controller::ControllerImpl::ProcessInput()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}

bool dae::Controller::ControllerImpl::IsPressedThisFrame(unsigned int button) const
{
	return m_ButtonsPressedThisFrame & button;
}

bool dae::Controller::ControllerImpl::IsUpThisFrame(unsigned int button) const
{
	return m_ButtonsReleasedThisFrame & button;
}

bool dae::Controller::ControllerImpl::IsDown(unsigned int button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}

bool dae::Controller::ControllerImpl::IsThumbsNotInDeadZone() const
{
	const SHORT thumbL{ m_CurrentState.Gamepad.sThumbLX };
	const SHORT thumbR{ m_CurrentState.Gamepad.sThumbRX };
	const float percentageThumbL{ std::abs(thumbL / static_cast<float>(SHRT_MIN)) };
	const float percentageThumbR{ std::abs(thumbR / static_cast<float>(SHRT_MAX)) };

	return (percentageThumbL > m_DeadzonePercentage) || (percentageThumbR > m_DeadzonePercentage);
}

dae::Controller::Controller() : m_pPimpl{ std::make_unique<ControllerImpl>() }
{
}

dae::Controller::~Controller() = default;

void dae::Controller::ProcessInput()
{
	m_pPimpl->ProcessInput();
}

bool dae::Controller::IsPressedThisFrame(unsigned int button) const
{
	return m_pPimpl->IsPressedThisFrame(button);
}

bool dae::Controller::IsUpThisFrame(unsigned int button) const
{
	return m_pPimpl->IsUpThisFrame(button);
}

bool dae::Controller::IsDown(unsigned int button) const
{
	return m_pPimpl->IsDown(button);
}

bool dae::Controller::IsThumbsNotInDeadZone() const
{
	return m_pPimpl->IsThumbsNotInDeadZone();
}
