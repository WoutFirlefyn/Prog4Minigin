#include <stdexcept>
#include <glm/glm.hpp>
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
#include "GameTime.h"

#define WIN32_LEAN_AND_MEAN 
#ifdef STEAM
#pragma warning (push)
#pragma warning (disable: 4996)
#include "steam_api.h"
#pragma warning (pop)
#endif // Steam

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
const glm::ivec2 dae::Minigin::m_WindowSize{ 640, 480 };
dae::Minigin::Minigin()
{
	srand(static_cast<unsigned int>(time(NULL)));
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_WindowSize.x,
		m_WindowSize.y,
		SDL_WINDOW_OPENGL
	);
	
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& time = GameTime::GetInstance();

	time.SetFPSCap(60.f);

	LONGLONG MS_PER_FRAME = static_cast<LONGLONG>(time.GetMsPerFrame());
	float FIXED_TIME_STEP = time.GetFixedTimeStep();

	auto lastTime = high_resolution_clock::now();
	float lag = 0.f;
	bool doContinue = true;
	while (doContinue)
	{
		const auto currentTime = high_resolution_clock::now();
		const float deltaTime = duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;
		lag += deltaTime;

		time.SetDeltaTime(deltaTime);

#ifdef STEAM
		SteamAPI_RunCallbacks();
#endif // Steam

		doContinue = input.ProcessInput();
		while (lag >= FIXED_TIME_STEP)
		{
			sceneManager.FixedUpdate();
			lag -= FIXED_TIME_STEP;
		}
		sceneManager.Update();
		sceneManager.LateUpdate();
		renderer.Render();
		sceneManager.RenderGUI();

		const auto sleepTime = currentTime + milliseconds(MS_PER_FRAME) - high_resolution_clock::now();
		std::this_thread::sleep_for(sleepTime);
	}
}
