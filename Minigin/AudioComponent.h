#pragma once
#include "Component.h"
#include "Audio.h"
#include "ServiceLocator.h"
namespace dae {
	static const std::string GameOverMusic{ "../Data/Sound/GameOverMusic.mp3" };
	static const std::string GameMusic{ "../Data/Sound/GameMusic.mp3" };
	static const std::string MenuMusic{ "../Data/Sound/ScoreMusic.mp3" };
	static const std::string HighscoreMusic{ "../Data/Sound/MenuMusic.wav" };
	static const std::string FindDoorMusic{ "../Data/Sound/FindTheDoorMusic.mp3" };

	class AudioComponent final : public Component
	{
	public:
		AudioComponent() = default;
		~AudioComponent() = default;
		AudioComponent(const AudioComponent&) = delete;
		AudioComponent(AudioComponent&&) noexcept = delete;
		AudioComponent& operator=(const AudioComponent&) = delete;
		AudioComponent& operator=(AudioComponent&&) noexcept = delete;

		virtual void Init() override;
		virtual void Update() override;

		void PlayPlayerDeathSound(bool shouldStopPreviousSound = false);
		void PlayBombSound(bool shouldStopPreviousSound = false);
		void PlayExplosionSound(bool shouldStopPreviousSound = false);
		void StopSound();

		void QueueMusic(std::string musicFile, bool resetInit = false)
		{
			m_SelectedMusicFile = musicFile;
			if (resetInit)
			{
				m_IsInit = true;
			}
		};

	private:
		Audio* m_pAudioService{ nullptr };

		int m_BombDropSoundId{ 0 };
		int m_BombExplosionSoundId{ 0 };
		int m_PlayerDeathSoundId{ 0 };

		std::string m_SelectedMusicFile{ "" };
		bool		m_IsInit{ false };

		void PlayMusic(std::string musicFile);
	};
}

