#pragma once
#include <memory>
#include <string>

namespace dae
{
	enum class ControllerButton : unsigned int
	{
		DPAD_UP = 0x0001,
		DPAD_DOWN = 0x0002,
		DPAD_LEFT = 0x0004,
		DPAD_RIGHT = 0x0008,
		START = 0x0010,
		BACK = 0x0020,
		LEFT_THUMB = 0x0040,
		RIGHT_THUMB = 0x0080,
		LEFT_SHOULDER = 0x0100,
		RIGHT_SHOULDER = 0x0200,
		A = 0x1000,
		B = 0x2000,
		X = 0x4000,
		Y = 0x8000,
	};

	class Command;
	enum class InputType;
	class Controller final
	{
	public:    

		Controller(uint8_t controllerIdx);
		~Controller();

		Controller(const Controller& other) = delete;
		Controller(Controller&& other) noexcept = delete;
		Controller& operator=(const Controller& other) = delete;
		Controller& operator=(Controller&& other) noexcept = delete;

		void ProcessInput();
		void BindCommand(std::unique_ptr<Command>&& pCommand, ControllerButton button, InputType triggerType);
		void ClearInputActions();

		bool IsPressedThisFrame(unsigned int button) const;
		bool IsUpThisFrame(unsigned int button) const;
		bool IsDown(unsigned int button) const;
		bool IsThumbsNotInDeadZone(unsigned int thumb) const;
	private:
		class ControllerImpl;
		std::unique_ptr<ControllerImpl> m_pControllerImpl;
	};
}

 
