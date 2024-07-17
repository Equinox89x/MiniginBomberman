#pragma once
#include <mutex>
#include <queue>
#include "Audio.h"
#include <thread>
#include <future>

class AudioMaker final : public Audio
{
public:
    AudioMaker();

    virtual void Update() override;

    virtual void StopSound(int channelId = -1) override;
    virtual void StopAllSounds() override;

    virtual void PlaySound(int soundID) override;
    virtual void PlayMusic() override;
    virtual void PauseMusic() override;
    virtual void ResumeMusic() override;

    virtual int LoadSound(const char* file) override;
    virtual void LoadMusic(const char* file) override;

    virtual void SetEffectVolume(int volume) override;
    virtual void SetMusicVolume(int volume) override;

private:
    void Initialize();

    bool m_Running;
    std::promise<void> m_StopSignal;
    Mix_Music* m_pMusic;
    std::mutex m_Mutex;
    std::condition_variable m_ConditionVariable;
    std::queue<int> m_Queue;
    std::vector<Mix_Chunk*> m_pSounds;
    std::jthread m_AudioThread;

};

class NullAudio : public Audio
{
public:
    virtual void Update() override {};
    virtual void PlaySound(int) override {}
    virtual void PlayMusic() override {}
    virtual void ResumeMusic() override {}

    virtual void StopSound(int /*channelId = -1*/) override {}
    virtual void StopAllSounds() override {}
    virtual void PauseMusic() override {}

    virtual int LoadSound(const char*) override { return 0; };
    virtual void LoadMusic(const char*) override {}

    virtual void SetEffectVolume(int) override {}
    virtual void SetMusicVolume(int) override {}
};