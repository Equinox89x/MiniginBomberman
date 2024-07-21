#include "EnemyComponent.h"
#include "Timer.h"
#include "EntityMovementComponent.h"
#include <ValuesComponent.h>
#include "Observers.h"
#include <TextObjectComponent.h>
#include "FloatingScoreComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"
#include <AudioComponent.h>
#include "PlayerComponent.h"

dae::EnemyComponent::~EnemyComponent()
{
	m_EnemyState.reset();

	m_Player = nullptr;
}

void dae::EnemyComponent::Init()
{
	SetState(new AliveState(m_Scene), MathLib::ELifeState::ALIVE);
}

void dae::EnemyComponent::Update()
{
	m_EnemyState->Update(GetGameObject());
}

void dae::EnemyComponent::Render() const
{
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 255, 255, 255);
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_Scoreect0);
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 255, 255);
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_Scoreect1);
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255);
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_Scoreect2);
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 0, 255);
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_Scoreect3);
}

//
//void dae::DeathState::Init()
//{
//	gameObject->GetComponent<dae::AudioComponent>()->PlayPopSound();
//
//	gameObject->GetComponent<EnemyComponent>()->SetLifeState(MathLib::ELifeState::DEAD);
//	auto font{ ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 10) };
//	auto pos{ gameObject->GetTransform()->GetWorld().Position };
//
//	auto rect{ gameObject->GetComponent<TextureComponent>()->GetRect() };
//
//	EnemyType == EEnemyType::Balloom ? m_Score = 200 : m_Score = 400;
//
//	if (MathLib::IsCompletelyOverlapping(m_ScoreRect1, rect)) {
//		EnemyType == EEnemyType::Balloom ? m_Score += 100 : m_Score += 200;
//	}
//	if (MathLib::IsCompletelyOverlapping(m_ScoreRect2, rect)) {
//		EnemyType == EEnemyType::Balloom ? m_Score += 200 : m_Score += 400;
//	}
//	if (MathLib::IsCompletelyOverlapping(m_ScoreRect3, rect)) {
//		EnemyType == EEnemyType::Balloom ? m_Score += 300 : m_Score += 600;
//	}
//
//	auto go{ std::make_shared<dae::GameObject>() };
//	go->AddComponent(std::make_unique<dae::TextObjectComponent>(std::to_string(m_Score), font));
//	go->AddComponent(std::make_unique<dae::FloatingScoreComponent>(m_Scene, m_Score, pos));
//	m_Scene->Add(go);
//
//	if (auto player{ gameObject->GetComponent<EnemyComponent>()->GetPlayer() }) {
//		player->GetComponent<ValuesComponent>()->IncreaseScore(m_Score);
//	}
//}
//
//void dae::DeathState::Update()
//{
//	m_DeathTimer -= Timer::GetInstance().GetDeltaTime();
//	if (m_DeathTimer <= 0) {
//		
//		gameObject->MarkForDestroy();
//	}
//}
//
//void dae::MovingState::Init()
//{
//	m_Timer = static_cast<float>(MathLib::CalculateChance(10));
//	if (gameObject) {
//		std::string enemy{ EnemyType == EEnemyType::Oneal ? "Oneal" : "Balloom" };
//		gameObject->GetComponent<TextureComponent>()->SetTexture("Enemies/" + enemy + "Left.png", 0.1f, 2);
//		gameObject->GetComponent<EntityMovementComponent>()->SetGhostModeEnabled(false);
//		gameObject->GetComponent<EntityMovementComponent>()->DisableMovement(false);
//		if (auto comp{ gameObject->GetComponent<EnemyComponent>() }) {
//			comp->SetLifeState(MathLib::ELifeState::ALIVE);
//		}
//	}
//}
//
//void dae::MovingState::Update()
//{
//	if (m_IsVersus) return;
//
//	m_Timer -= Timer::GetInstance().GetDeltaTime();
//	if (m_Timer > 0) return;
//	if (MathLib::CalculateChance() >= 80) {
//		if (EnemyType == EEnemyType::Oneal) {
//			gameObject->GetComponent<EnemyComponent>()->SetState(new GhostState());
//		}
//		else {
//			gameObject->GetComponent<EnemyComponent>()->SetState(new GhostState());
//		}
//	}
//}
