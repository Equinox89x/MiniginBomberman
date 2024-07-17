#pragma once
#include "Component.h"
#include "Audio.h"
#include "ServiceLocator.h"
namespace dae {
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
		void PlayPumpSound(bool shouldStopPreviousSound = false);
		void PlayShootSound(bool shouldStopPreviousSound = false);
		void PlayFireSound(bool shouldStopPreviousSound = false);
		void PlayGhostSound(bool shouldStopPreviousSound = false);
		void PlayPopSound(bool shouldStopPreviousSound = false);
		void PlayGameMusicSound();
		void PlayRockSound(bool shouldStopPreviousSound = false);
		void StopSound();

	private:
		Audio* m_pAudioService{ nullptr };

		int m_PumpSoundId{ 0 };
		int m_ShootSoundId{ 0 };
		int m_FiresoundId{ 0 };
		int m_GhostSoundId{ 0 };
		int m_PopSoundId{ 0 };
		int m_PlayerDeathSoundId{ 0 };
		int m_RockSoundId{ 0 };

	};
}

