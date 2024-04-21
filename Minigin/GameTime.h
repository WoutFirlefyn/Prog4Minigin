#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Singleton.h"

//-----------------------------------------------------
// Time Class									
//-----------------------------------------------------
namespace dae
{
	class GameTime final : public Singleton<GameTime>
	{
	public:
		void SetFPSCap(float maxFPS) { m_MsPerFrame = 1000.f / maxFPS; }
		void SetDeltaTime(float deltaTime) { m_DeltaTime = deltaTime; };
		float GetDeltaTime() const { return m_DeltaTime; };
		float GetMsPerFrame() const { return m_MsPerFrame; };
		float GetFixedTimeStep() const { return m_FixedTimeStep; };

	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------


		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		friend class Singleton<GameTime>;
		GameTime() = default;
		float m_DeltaTime{};
		float m_MsPerFrame{ 1000.f / 60.f };
		float m_FixedTimeStep{ 0.02f };
	};
}

 
