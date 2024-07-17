#include "RotatorComponent.h"
#include "Timer.h"

dae::RotatorComponent::RotatorComponent()
{

}

void dae::RotatorComponent::Update()
{
	if (GetGameObject()) {
		if (GetGameObject()->GetTransform()) {
			GetGameObject()->GetTransform()->Rotate(m_Angle);
			m_Angle += Timer::GetInstance().GetDeltaTime() * m_Speed;
			if (m_Angle > 360) {
				m_Angle = 0;
			}
		}
	}
}
