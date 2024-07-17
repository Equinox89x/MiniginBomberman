#include "FireComponent.h"
#include "Timer.h"
#include "EnemyComponent.h"
#include "EntityMovementComponent.h"

void dae::FireComponent::Update()
{
	if (m_EnemyGameObject->IsMarkedForDestroy() && !GetGameObject()->IsMarkedForDestroy()) {
		GetGameObject()->MarkForDestroy();
	}

	m_Timer -= Timer::GetInstance().GetDeltaTime();
	if (m_Timer <= 0.f) {
		m_FireFinished = true;
	}
	else if (m_Timer <= 0.2f) {
		//GetGameObject()->GetComponent<TextureComponent>()->SetTexture("Enemies/Flame3.png", 0.1f, 1);
		GetGameObject()->GetComponent<TextureComponent>()->SetTexture("Enemies/Flame3"+m_Direction+".png", 0.1f, 1);
		if(m_Direction == "Left"){
			GetGameObject()->GetTransform()->TranslateWorld(m_OriginLocation.x - GetGameObject()->GetComponent<TextureComponent>()->GetRect().w, m_OriginLocation.y);
		}
	}
	else if (m_Timer <= 0.4f) {
		//GetGameObject()->GetComponent<TextureComponent>()->SetTexture("Enemies/Flame2.png", 0.1f, 1);
		GetGameObject()->GetComponent<TextureComponent>()->SetTexture("Enemies/Flame2" + m_Direction + ".png", 0.1f, 1);
		if (m_Direction == "Left") {
			GetGameObject()->GetTransform()->TranslateWorld(m_OriginLocation.x - GetGameObject()->GetComponent<TextureComponent>()->GetRect().w, m_OriginLocation.y);
		}
	}
	else if (m_Timer <= 0.6f) {
		GetGameObject()->GetComponent<TextureComponent>()->SetTexture("Enemies/Flame1" + m_Direction + ".png", 0.1f, 1);
		if (m_Direction == "Left") {
			GetGameObject()->GetTransform()->TranslateWorld(m_OriginLocation.x - GetGameObject()->GetComponent<TextureComponent>()->GetRect().w, m_OriginLocation.y);
		}
		//GetGameObject()->GetComponent<TextureComponent>()->SetTexture("Enemies/Flame1.png", 0.1f, 1);
	}
}

void dae::FireComponent::Init()
{
	auto comp{ GetGameObject()->AddComponent(std::make_unique<TextureComponent>()) };
	if (comp) {
		auto direction{ m_EnemyGameObject->GetComponent<EntityMovementComponent>()->GetDirection() };

		GetGameObject()->GetTransform()->TranslateWorld(m_OriginLocation);
		GetGameObject()->GetTransform()->SetScale(glm::vec2{ 2.4f, 2.4f });
		if(direction == MathLib::Movement::LEFT){
			//GetGameObject()->GetTransform()->SetScale(glm::vec2{ -2.4f, 2.4f });
			m_Direction = "Left";
		}
		else{
			//GetGameObject()->GetTransform()->SetScale(glm::vec2{ 2.4f, 2.4f });
			m_Direction = "Right";
			GetGameObject()->GetTransform()->AddTranslateWorld(static_cast<float>(m_EnemyGameObject->GetComponent<TextureComponent>()->GetRect().w), 0);
		}
		//comp->SetTexture("Enemies/Flame0.png", 0.1f, 1);
		comp->SetTexture("Enemies/Flame0"+m_Direction+".png", 0.1f, 1);
	}

}
