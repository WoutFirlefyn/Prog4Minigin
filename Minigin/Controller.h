#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include <memory>

//-----------------------------------------------------
// Controller Class									
//-----------------------------------------------------
namespace dae
{
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
		void BindCommand(std::unique_ptr<Command>&& pCommand, unsigned int button, InputType triggerType);
		
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

 
