#pragma once

namespace dae {
    class Event final
    {
    public:
        Event() = default;
        Event(int id) : m_EventId{ id } {}

        int GetEvent() const { return m_EventId; }
    private:
        int m_EventId{};
    };

    enum EventType : int {
        Achievement,
        Live,
        GameOver,
        Score,
        EnemyDeath,
        Reset,
        RockDeath,
    };
}