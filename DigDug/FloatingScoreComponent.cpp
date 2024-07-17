#include "FloatingScoreComponent.h"

void dae::FloatingScoreComponent::Init()
{
	GetGameObject()->GetTransform()->Translate(m_Position);
}

void dae::FloatingScoreComponent::Update()
{
	auto dt{ Timer::GetInstance().GetDeltaTime() };
	float speed{ dt * m_Speed };
	GetGameObject()->GetTransform()->AddTranslate(0, -speed);
	m_Timer -= dt;
	if (m_Timer < 0) {
		m_Timer = 50;
		GetGameObject()->MarkForDestroy();
	}
}

