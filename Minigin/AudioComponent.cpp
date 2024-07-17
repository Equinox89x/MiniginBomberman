#include "AudioComponent.h"

void dae::AudioComponent::Init()
{
	m_pAudioService = ServiceLocator::getAudio();
	if (m_pAudioService == nullptr) return;

	m_PumpSoundId = m_pAudioService->LoadSound("../Data/Sound/Pump.wav");
	m_ShootSoundId = m_pAudioService->LoadSound("../Data/Sound/ShootSound.wav");
	m_FiresoundId = m_pAudioService->LoadSound("../Data/Sound/Fire.wav");
	m_GhostSoundId = m_pAudioService->LoadSound("../Data/Sound/EnemyMoving.wav");
	m_PopSoundId = m_pAudioService->LoadSound("../Data/Sound/EnemyPop.wav");
	m_PlayerDeathSoundId = m_pAudioService->LoadSound("../Data/Sound/PlayerDeathSound.wav");
	m_RockSoundId = m_pAudioService->LoadSound("../Data/Sound/RockSound.mp3");
	m_pAudioService->LoadMusic(std::string("../Data/Sound/GameMusic.mp3").c_str());
}

void dae::AudioComponent::Update()
{
	if (m_pAudioService == nullptr) {
		m_pAudioService = ServiceLocator::getAudio();
		if (m_pAudioService == nullptr) return;

		m_PumpSoundId = m_pAudioService->LoadSound("../Data/Sound/Pump.wav");
		m_ShootSoundId = m_pAudioService->LoadSound("../Data/Sound/ShootSound.wav");
		m_FiresoundId = m_pAudioService->LoadSound("../Data/Sound/Fire.wav");
		m_GhostSoundId = m_pAudioService->LoadSound("../Data/Sound/EnemyMoving.wav");
		m_PopSoundId = m_pAudioService->LoadSound("../Data/Sound/EnemyPop.wav");
		m_PlayerDeathSoundId = m_pAudioService->LoadSound("../Data/Sound/PlayerDeathSound.wav");
		m_RockSoundId = m_pAudioService->LoadSound("../Data/Sound/RockSound.mp3");
		m_pAudioService->LoadMusic(std::string("../Data/Sound/GameMusic.mp3").c_str());
		PlayGameMusicSound();
	}
}

void dae::AudioComponent::PlayPlayerDeathSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(m_PlayerDeathSoundId);
}

void dae::AudioComponent::PlayPumpSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(m_PumpSoundId);
}

void dae::AudioComponent::PlayShootSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(m_ShootSoundId);
}

void dae::AudioComponent::PlayFireSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(m_FiresoundId);
}

void dae::AudioComponent::PlayGhostSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(m_GhostSoundId);
}

void dae::AudioComponent::PlayPopSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(m_PopSoundId);
}

void dae::AudioComponent::PlayGameMusicSound()
{
	m_pAudioService->PlayMusic();
	m_pAudioService->SetEffectVolume(60);
}

void dae::AudioComponent::PlayRockSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(m_RockSoundId);
}

void dae::AudioComponent::StopSound()
{
	m_pAudioService->StopSound();
}
