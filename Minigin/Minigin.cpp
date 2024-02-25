#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <chrono>
#include <thread>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"

using namespace std::chrono;

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

//void dae::Minigin::Run(const std::function<void()>& load)
//{
//	load();
//
//	auto& renderer = Renderer::GetInstance();
//	auto& sceneManager = SceneManager::GetInstance();
//	auto& input = InputManager::GetInstance();

	// todo: this update loop could use some work.
	//bool do_continue = true;
	//auto last_time = high_resolution_clock::now();
	//float lag = 0.f;

	//const float fixed_time_step = 0.03333f;
	//duration<float> ms_per_frame(1 / 60.f);

	//while (do_continue)
	//{
	//	const auto current_time = high_resolution_clock::now();
	//	const float delta_time = duration<float>(current_time - last_time).count();
	//	last_time = current_time;
	//	lag += delta_time;

	//	do_continue = input.ProcessInput();
	//	while (lag >= fixed_time_step)
	//	{
	//		sceneManager.Update(fixed_time_step);
	//		lag -= fixed_time_step;
	//	}
	//	sceneManager.Update(delta_time);
	//	renderer.Render();

	//	const auto sleep_time = current_time + ms_per_frame - high_resolution_clock::now();

	//	std::this_thread::sleep_for(sleep_time);
	//}
//}
void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();

	constexpr LONGLONG MS_PER_FRAME = static_cast<LONGLONG>(1000.0f / 60.0f);
	constexpr float FIXED_TIME_STEP = 20.f;

	auto lastTime = high_resolution_clock::now();
	float lag = 0.f;
	bool doContinue = true;
	while (doContinue)
	{
		const auto currentTime = high_resolution_clock::now();
		const float deltaTime = duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;
		lag += deltaTime;

		doContinue = input.ProcessInput();
		while (lag >= FIXED_TIME_STEP)
		{
			sceneManager.Update(FIXED_TIME_STEP);
			lag -= FIXED_TIME_STEP;
		}
		sceneManager.Update(deltaTime);
		renderer.Render();

		const auto sleepTime = currentTime + milliseconds(MS_PER_FRAME) - high_resolution_clock::now();
		std::this_thread::sleep_for(sleepTime);
	}
}
