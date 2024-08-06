#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "Timer.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"

#include <chrono>
#include <thread>
#include "GameObject.h"
#include "Audio.h"
#include "ServiceLocator.h"
//#include <steam_api.h>

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
		"DigDug - Bram Robyn - Prog4 Exam",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WindowSizeX,
		WindowSizeY,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	Timer::GetInstance().Init();

	ResourceManager::GetInstance().Init(dataPath);
}

dae::Minigin::~Minigin()
{
	ServiceLocator::Quit();
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();


}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto sound{ new AudioMaker() };
	ServiceLocator::provide(sound);

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	//auto& input = InputManager::GetInstance();

	auto input = InputCollection(3);

	bool doContinue = true;
	auto lastTime{ std::chrono::high_resolution_clock::now() };
	float lag{ 0 };

	std::thread audioThread(&Audio::Update, sound);
	audioThread.join();
	Timer::GetInstance().Start();
	sceneManager.Init();
	while (doContinue)
	{
		const auto currentTime{ std::chrono::high_resolution_clock::now() };
		//const float deltaTime{ std::chrono::duration<float>(currentTime - lastTime).count()};
		//lastTime = currentTime;
		//lag += deltaTime;

		Timer::GetInstance().Update();

		doContinue = input.ProcessInput();
		while (lag >= fixedTimeStep) {
			sceneManager.LateUpdate();
			lag -= fixedTimeStep;
		}

		if (!doContinue) {
			sceneManager.Cleanup();
		}
		sceneManager.Update();
		//SteamAPI_RunCallbacks();
		renderer.Render();
		sceneManager.LateUpdate();

		const auto sleepTime = currentTime + std::chrono::milliseconds(msPerFrame) - std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(sleepTime);
	}


	input.Cleanup();
	Timer::GetInstance().Stop();
	ServiceLocator::getAudio()->StopAllSounds();
	ServiceLocator::getAudio()->StopMusic();
}
