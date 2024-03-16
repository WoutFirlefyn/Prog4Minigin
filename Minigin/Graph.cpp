//---------------------------
// Includes
//---------------------------
#include "Graph.h"
#include "imgui_plot.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::Graph::Graph()
{
}

dae::Graph::~Graph()
{
	// nothing to destroy
}

void dae::Graph::ShowExercise1()
{
	ImGui::Begin("Exercise 1", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	static int numOfSamples{ 15 };
	ImGui::InputInt("# samples", &numOfSamples);

	if (ImGui::Button("Trash the cache with ints"))
		TrashTheCache<int>(numOfSamples);

	if (m_IntTimings.size() > 0)
	{
		ImGui::PlotConfig conf{};
		conf.values.xs = m_XAxis.data();
		conf.values.ys = m_IntTimings.data();
		conf.values.count = static_cast<int>(m_IntTimings.size());
		conf.scale.min = 0;
		conf.scale.max = static_cast<float>(m_IntTimings[0]);
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%.2f, y=%.2f";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 2.f;

		ImGui::Plot("Int", conf);
	}
	ImGui::End();
}

void dae::Graph::ShowExercise2()
{
	ImGui::Begin("Exercise 2", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	static int numOfSamples{ 15 };
	ImGui::InputInt("# samples", &numOfSamples);

	if (ImGui::Button("Trash the cache with Gameobjects"))
		TrashTheCache<GameObject3D>(numOfSamples);

	ImU32 colors[2] = { ImColor(255, 0, 0), ImColor(0, 0, 255) };
	ImGui::PlotConfig conf{};
	if (m_GO3DTimings.size() > 0)
	{
		conf.values.xs = m_XAxis.data();
		conf.values.ys = m_GO3DTimings.data();
		conf.values.count = static_cast<int>(m_GO3DTimings.size());
		conf.values.color = colors[0];
		conf.scale.min = 0;
		conf.scale.max = static_cast<float>(m_GO3DTimings[0]);
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%.2f, y=%.2f";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 2.f;

		ImGui::Plot("GameObjects", conf);
	}

	if (ImGui::Button("Trash the cache with GameobjectsAlt"))
		TrashTheCache<GameObject3DAlt>(numOfSamples);

	if (m_GO3DAltTimings.size() > 0)
	{
		conf.values.xs = m_XAxis.data();
		conf.values.ys = m_GO3DAltTimings.data();
		conf.values.count = static_cast<int>(m_GO3DAltTimings.size());
		conf.values.color = colors[1];
		conf.scale.min = 0;
		conf.scale.max = static_cast<float>(m_GO3DAltTimings[0]);
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%.2f, y=%.2f";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 2.f;

		ImGui::Plot("GameObjectsAlt", conf);
	}

	if (m_GO3DTimings.size() > 0 && m_GO3DAltTimings.size() > 0)
	{
		ImGui::SeparatorText("Combined");
		const float* YDataList[] = { m_GO3DTimings.data(), m_GO3DAltTimings.data() };

		conf.values.ys = nullptr; 
		conf.values.ys_list = YDataList;
		conf.values.ys_count = 2;
		conf.values.colors = colors;
		conf.scale.max = m_GO3DTimings[0];

		ImGui::Plot("Combined", conf);
	}

	ImGui::End();
}

//---------------------------
// Member functions
//---------------------------

// write member functions here




