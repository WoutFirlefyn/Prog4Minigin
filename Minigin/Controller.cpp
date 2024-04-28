//---------------------------
// Includes
//---------------------------
#include <windows.h>
#include <Xinput.h>
#include <vector>
#include <cmath>
#include "Input.h"
#include "Command.h"
#include "Controller.h"

class dae::Controller::ControllerImpl
{
public:
	ControllerImpl(uint8_t controllerIdx) : m_ControllerIndex{ controllerIdx }
	{
	}
	~ControllerImpl() = default;

	ControllerImpl(const ControllerImpl& other) = delete;
	ControllerImpl(ControllerImpl&& other) noexcept = delete;
	ControllerImpl& operator=(const ControllerImpl& other) = delete;
	ControllerImpl& operator=(ControllerImpl&& other) noexcept = delete;

	void ProcessInput();
	void BindCommand(std::unique_ptr<Command>&& pCommand, unsigned int button, InputType triggerType);

	bool IsPressedThisFrame(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsDown(unsigned int button) const;
	bool IsThumbsNotInDeadZone() const;

	uint8_t GetIndex() const { return m_ControllerIndex; }
private:
	std::vector<InputAction> m_vControllerInputAction{};
	uint8_t m_ControllerIndex{};
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

	for (const InputAction& inputAction : m_vControllerInputAction)
	{
		switch (inputAction.InputType)
		{
		case InputType::Down:
			if (IsDown(inputAction.Button))
				inputAction.pCommand->Execute();
			break;
		case InputType::Released:
			if (IsUpThisFrame(inputAction.Button))
				inputAction.pCommand->Execute();
			break;
		case InputType::Pressed:
			if (IsPressedThisFrame(inputAction.Button))
				inputAction.pCommand->Execute();
			break;
		case InputType::Joystick:
			if (IsThumbsNotInDeadZone())
				inputAction.pCommand->Execute();
			break;
		}
	}
}

void dae::Controller::ControllerImpl::BindCommand(std::unique_ptr<Command>&& pCommand, unsigned int button, InputType triggerType)
{
	m_vControllerInputAction.push_back(dae::InputAction(std::move(pCommand), button, triggerType));
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

dae::Controller::Controller(uint8_t controllerIdx) : m_pControllerImpl{ std::make_unique<ControllerImpl>(controllerIdx) }
{
}

dae::Controller::~Controller() = default;

void dae::Controller::ProcessInput()
{
	m_pControllerImpl->ProcessInput();
}

void dae::Controller::BindCommand(std::unique_ptr<Command>&& pCommand, ControllerButton button, InputType triggerType)
{
	m_pControllerImpl->BindCommand(std::move(pCommand), static_cast<unsigned int>(button), triggerType);
}

bool dae::Controller::IsPressedThisFrame(unsigned int button) const
{
	return m_pControllerImpl->IsPressedThisFrame(button);
}

bool dae::Controller::IsUpThisFrame(unsigned int button) const
{
	return m_pControllerImpl->IsUpThisFrame(button);
}

bool dae::Controller::IsDown(unsigned int button) const
{
	return m_pControllerImpl->IsDown(button);
}

bool dae::Controller::IsThumbsNotInDeadZone() const
{
	return m_pControllerImpl->IsThumbsNotInDeadZone();
}
