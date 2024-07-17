#include "AudioMaker.h"
#include <iostream>

AudioMaker::AudioMaker() : 
	m_Running(true)
{
	Initialize();
}

void AudioMaker::Initialize()
{
	int flags = MIX_INIT_MP3;
	int result = Mix_Init(flags);
	if ((result & flags) != flags) {
		printf("Mix_Init: Failed to init required ogg and mod support!\n");
		printf("Mix_Init: %s\n", Mix_GetError());
	}

	// start SDL with audio support
	if (SDL_Init(SDL_INIT_AUDIO) == -1) {
		printf("SDL_Init: %s\n", SDL_GetError());
		exit(1);
	}
	// open 44.1KHz, signed 16bit, system byte order,
	//      stereo audio, using 1024 byte chunks
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		exit(2);
	}
	// 8 mixing channels get created
	MIX_CHANNELS;


}

void AudioMaker::Update()
{
	m_AudioThread = std::jthread([this]() {
		while (m_Running)
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_ConditionVariable.wait(lock);

			while (!m_Queue.empty() && m_Running)
			{
				int id = m_Queue.front();
				m_Queue.pop();
				Mix_PlayChannel(-1, m_pSounds.at(id), 0);
			}
			lock.unlock();
		}
		std::cout << "Exiting console audio update" << std::endl;
	});

	m_AudioThread.detach();
}

void AudioMaker::PlaySound(int soundID)
{
	//Mix_PlayChannel(-1, m_pSounds.at(soundID), 0);
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_Queue.push(soundID);
	m_ConditionVariable.notify_one();
}

void AudioMaker::StopSound(int channelId)
{
	Mix_HaltChannel(channelId);
}

void AudioMaker::StopAllSounds()
{
	//Dont use mix functions after this, else call openAudio first
	Mix_CloseAudio();

	m_Running = false;
	m_ConditionVariable.notify_one();

	if (m_AudioThread.joinable())
		m_AudioThread.join();
}

void AudioMaker::PlayMusic()
{
	Mix_PlayMusic(m_pMusic, -1);
}

void AudioMaker::PauseMusic()
{
	Mix_PauseMusic();
}

void AudioMaker::ResumeMusic()
{
	Mix_ResumeMusic();
}

int AudioMaker::LoadSound(const char* file)
{
	auto sound{ Mix_LoadWAV(file) };
	m_pSounds.push_back(sound);
	return int(m_pSounds.size()) - 1;
}

void AudioMaker::LoadMusic(const char* file)
{
	m_pMusic = Mix_LoadMUS(file);
	if (m_pMusic == nullptr)
	{
		std::string errorMsg = "SoundStream: Failed to load " + std::string{ file } + ",\nSDL_mixer Error: " + Mix_GetError();
		std::cerr << errorMsg;
	}
}

void AudioMaker::SetEffectVolume(int volume)
{
	Mix_Volume(-1, volume);
}

void AudioMaker::SetMusicVolume(int volume)
{
	Mix_VolumeMusic(volume);
}