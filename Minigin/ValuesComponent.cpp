#include "ValuesComponent.h"
#include "Subject.h"
#include "Event.h"

void dae::ValuesComponent::Damage()
{
	m_Lives--;
	Event live{ EventType::Live };
	Notify(GetGameObject(), live);
	if (m_Lives <= 0) {
		Event gameOver{ EventType::GameOver };
		Notify(GetGameObject(), gameOver);
	}
}

void dae::ValuesComponent::IncreaseScore(int score)
{
	m_Score += score;
	auto go{ GetGameObject() };
	Event scoreEvent{ EventType::Score };
	Notify(go, scoreEvent);
}

int dae::ValuesComponent::GetLives() const
{
	return m_Lives;
}

void dae::ValuesComponent::SetLives(int lives)
{
	m_Lives = lives;
}

int dae::ValuesComponent::GetScores() const
{
	return m_Score;
}

void dae::ValuesComponent::ResetObserver()
{
	Event reset{ EventType::Reset };
	Notify(GetGameObject(), reset);
}

void dae::ValuesComponent::Reset()
{

	//m_Lives = 3;
	//m_Score = 0;
	//NrOfHits = 0;
	//NrOfShotsFired = 0;
}

void dae::ValuesComponent::GameEnd()
{
	m_Lives = 3;
	m_Score = 0;
	NrOfHits = 0;
	NrOfShotsFired = 0;
}