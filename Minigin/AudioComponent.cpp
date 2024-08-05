#include "AudioComponent.h"

void dae::AudioComponent::Init()
{
	
	if (m_pAudioService == nullptr)
		m_pAudioService = ServiceLocator::getAudio();
	
	if (m_pAudioService == nullptr)
		return;

	m_IsInit = true;

	m_PlayerDeathSoundId = m_pAudioService->LoadSound("../Data/Sound/PlayerDeathSound.wav");
	m_BombExplosionSoundId = m_pAudioService->LoadSound("../Data/Sound/Explosion.mp3");
	m_BombDropSoundId = m_pAudioService->LoadSound("../Data/Sound/BombDropSound.wav");
}

void dae::AudioComponent::Update()
{
	if (m_pAudioService == nullptr) {
		m_pAudioService = ServiceLocator::getAudio();
		if (m_pAudioService == nullptr) return;

		m_IsInit = true;

		m_PlayerDeathSoundId = m_pAudioService->LoadSound("../Data/Sound/PlayerDeathSound.wav");
		m_BombExplosionSoundId = m_pAudioService->LoadSound("../Data/Sound/Explosion.mp3");
		m_BombDropSoundId = m_pAudioService->LoadSound("../Data/Sound/BombDropSound.wav");

	}

	PlayMusic(m_SelectedMusicFile);
}

void dae::AudioComponent::PlayPlayerDeathSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(m_PlayerDeathSoundId);
}

void dae::AudioComponent::PlayBombSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(m_BombDropSoundId);
}

void dae::AudioComponent::PlayExplosionSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(100);
	m_pAudioService->PlaySound(m_BombExplosionSoundId);
}

void dae::AudioComponent::PlayMusic(std::string musicFile)
{
	if (m_IsInit && musicFile != "")
	{
		m_pAudioService->LoadMusic(musicFile.c_str());
		m_pAudioService->PlayMusic();
		m_pAudioService->SetEffectVolume(60);
		m_IsInit = false;
	}
}

void dae::AudioComponent::StopSound()
{
	m_pAudioService->StopSound();
}
