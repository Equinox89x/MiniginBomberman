#include "RockComponent.h"
#include "EnemyComponent.h"
#include "MathLib.h"
#include "Observers.h"
#include <TextObjectComponent.h>
#include "FloatingScoreComponent.h"
#include "ResourceManager.h"
#include <AudioComponent.h>
#include "PlayerComponent.h"

void dae::RockComponent::Update()
{
	if (GetGameObject()->IsMarkedForDestroy()) return;
	if (IsMarkedForDestroy()) return;
	auto players{ m_Scene->GetGameObjects(EnumStrings[Names::PlayerGeneral], false) };
	auto rect{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
	rect.y += rect.h;
	rect.w /= 2;
	rect.x+= rect.w;
	rect.h /= 2;

	bool isOverlapping{ false };
	if (!m_Skipcheck) {
		for (const auto& player : players) {
			if (MathLib::IsOverlapping(rect, player->GetComponent<TextureComponent>()->GetRect())) {
				m_PlayerHasPassed = true;
				isOverlapping = true;
			}
		}
	}

	if (m_PlayerHasPassed && !isOverlapping) {
		m_Skipcheck = true;
		GetGameObject()->GetComponent<AudioComponent>()->PlayRockSound();
	}

	if (m_Skipcheck) {
		auto dt{ Timer::GetInstance().GetDeltaTime() };
		m_TimeBeforeFall -= dt;
		if (m_TimeBeforeFall < 0) {

			for (const auto& player : players) {
				if (MathLib::IsOverlapping(rect, player->GetComponent<TextureComponent>()->GetRect())) {
					//player->Die();
				}
			}

			auto enemies{ m_Scene->GetGameObjects(EnumStrings[Names::EnemyGeneral], false) };

			for (const auto& enemy : enemies) {
				if (MathLib::IsOverlapping(rect, enemy->GetComponent<TextureComponent>()->GetRect())) {
					auto comp{ enemy->GetComponent<EnemyComponent>() };
					if (comp->GetLifeState() != MathLib::ELifeState::DEAD) {
						comp->SetState(new DeathState());
						m_EnemiesHit++;
					}
				}
			}

			for (const auto& player : players) {
				if (MathLib::IsOverlapping(rect, player->GetComponent<TextureComponent>()->GetRect())) {
					auto comp{ player->GetComponent<PlayerComponent>() };
					comp->Respawn();
				}
			}


			if (m_MoveTimer > 0) {
				m_MoveTimer -= dt;
				GetGameObject()->GetTransform()->AddTranslate(0, m_Speed * dt);
			}
			else {
				m_DeathTimer -= dt;
				if (m_DeathTimer < 0) {
					auto font{ ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 10) };

					auto go{ std::make_shared<dae::GameObject>() };
					go->AddComponent(std::make_unique<TextObjectComponent>(std::to_string(GetScore()), font));
					go->AddComponent(std::make_unique<FloatingScoreComponent>(m_Scene, GetScore(), GetGameObject()->GetTransform()->GetFullPosition()));
					m_Scene->Add(go);

					m_PlayerHasPassed = false;
					m_Skipcheck = false;
					m_DeathTimer = 999;

					Event scoreEvent{ EventType::RockDeath };
					Notify(GetGameObject(), scoreEvent);
					//GetGameObject()->MarkForDestroy();
					
				}
			}
		}

	}
}

