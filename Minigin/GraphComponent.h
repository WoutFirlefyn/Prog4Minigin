#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include <memory>
#include <chrono>
#include <vector>
#include <numeric>
#include "BaseComponent.h"

//-----------------------------------------------------
// GraphComponent Class									
//-----------------------------------------------------
namespace dae
{
	struct transform
	{
		float matrix[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};
	};

	class GameObject3D
	{
	public:
		transform local{};
		int id{ 1 };

		GameObject3D& operator*=(int rhs)
		{
			id *= rhs;
			return *this;
		}
	};

	class GameObject3DAlt
	{
	public:
		transform* local{};
		int id{ 1 };

		GameObject3DAlt& operator*=(int rhs)
		{
			id *= rhs;
			return *this;
		}
	};

	class GameObject;
	class GraphComponent final : public BaseComponent
	{
	public:
		GraphComponent(GameObject* pGameObject);				// Constructor
		virtual ~GraphComponent() override = default;				// Destructor

		// -------------------------
		// Copy/move constructors and assignment operators
		// -------------------------    
		GraphComponent(const GraphComponent& other) = delete;
		GraphComponent(GraphComponent&& other) noexcept = delete;
		GraphComponent& operator=(const GraphComponent& other) = delete;
		GraphComponent& operator=(GraphComponent&& other) noexcept = delete;

		//-------------------------------------------------
		// Member functions						
		//-------------------------------------------------
		virtual void RenderGUI() override;

	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------
		template <typename T>
		void TrashTheCache(int sampleSize)
		{
			std::vector<float> newValues{};
			const int arrSize{ static_cast<int>(powf(2,22)) };
			std::unique_ptr<T[]> arr = std::make_unique<T[]>(arrSize);
			std::vector<float> timings{};
			for (int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
			{
				for (int amountOfTests{}; amountOfTests < sampleSize; ++amountOfTests)
				{
					const auto start = std::chrono::high_resolution_clock::now();
					for (int i{}; i < arrSize; i += stepsize)
						arr[i] *= 2;
					const auto end = std::chrono::high_resolution_clock::now();
					const auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
					timings.emplace_back(static_cast<float>(elapsedTime));
				}

				timings.erase(std::ranges::min_element(timings));
				timings.erase(std::ranges::max_element(timings));

				float sum = std::accumulate(timings.begin(), timings.end(), 0.f);
				float average = sum / timings.size();

				newValues.push_back(average);

				timings.clear();
			}

			if (std::is_same_v<T, int>)
				m_IntTimings = newValues;
			else if (std::is_same_v<T, GameObject3D>)
				m_GO3DTimings = newValues;
			else
				m_GO3DAltTimings = newValues;
		}

		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		std::vector<float> m_IntTimings{};
		std::vector<float> m_GO3DTimings{};
		std::vector<float> m_GO3DAltTimings{};
		const std::vector<float> m_XAxis{ 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };
		static const int m_Size{ 11 };

	};
}

 
