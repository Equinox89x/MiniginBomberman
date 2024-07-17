#include "EnemyComponent.h"
#include "Timer.h"
#include "EntityMovementComponent.h"
#include "FireComponent.h"
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
	delete m_State;
	m_State = nullptr;

	m_Player = nullptr;
}

void dae::EnemyComponent::Update()
{
	m_State->Update();

	//if (m_Scene) {
	//	auto players{ m_Scene->GetGameObjects(EnumStrings[Names::PlayerGeneral], false) };
	//	for (const auto& player : players) {
	//		if (MathLib::IsOverlapping(GetGameObject()->GetComponent<TextureComponent>()->GetRect(), player->GetComponent<TextureComponent>()->GetRect())) {
	//			auto comp{ player->GetComponent<PlayerComponent>() };
	//			comp->Respawn();
	//		}
	//	}
	//}
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

void dae::EnemyComponent::Init()
{
	SetState(new MovingState());
}

bool dae::EnemyComponent::PumpUp()
{
	m_PlayerState = MathLib::INFLATING;
	currentPumpStage++;
	GetGameObject()->GetComponent<TextureComponent>()->SetFrame(currentPumpStage);
	if (currentPumpStage >= maxPumpStage) {
		SetState(new DeathState());
		Event scoreEvent{ EventType::EnemyDeath };
		Notify(GetGameObject(), scoreEvent);
	}
	return currentPumpStage >= maxPumpStage;
}

void dae::DeathState::Init()
{
	gameObject->GetComponent<dae::AudioComponent>()->PlayPopSound();

	gameObject->GetComponent<EnemyComponent>()->SetLifeState(MathLib::ELifeState::DEAD);
	auto font{ ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 10) };
	auto pos{ gameObject->GetTransform()->GetFullPosition() };

	auto rect{ gameObject->GetComponent<TextureComponent>()->GetRect() };

	EnemyType == EEnemyType::Pooka ? m_Score = 200 : m_Score = 400;

	if (MathLib::IsCompletelyOverlapping(m_ScoreRect1, rect)) {
		EnemyType == EEnemyType::Pooka ? m_Score += 100 : m_Score += 200;
	}
	if (MathLib::IsCompletelyOverlapping(m_ScoreRect2, rect)) {
		EnemyType == EEnemyType::Pooka ? m_Score += 200 : m_Score += 400;
	}
	if (MathLib::IsCompletelyOverlapping(m_ScoreRect3, rect)) {
		EnemyType == EEnemyType::Pooka ? m_Score += 300 : m_Score += 600;
	}

	auto go{ std::make_shared<dae::GameObject>() };
	go->AddComponent(std::make_unique<dae::TextObjectComponent>(std::to_string(m_Score), font));
	go->AddComponent(std::make_unique<dae::FloatingScoreComponent>(m_Scene, m_Score, pos));
	m_Scene->Add(go);

	if (auto player{ gameObject->GetComponent<EnemyComponent>()->GetPlayer() }) {
		player->GetComponent<ValuesComponent>()->IncreaseScore(m_Score);
	}
}

void dae::DeathState::Update()
{
	m_DeathTimer -= Timer::GetInstance().GetDeltaTime();
	if (m_DeathTimer <= 0) {
		
		gameObject->MarkForDestroy();
	}
}

void dae::MovingState::Init()
{
	m_Timer = static_cast<float>(MathLib::CalculateChance(10));
	if (gameObject) {
		std::string enemy{ EnemyType == EEnemyType::Fygar ? "Fygar" : "Pooka" };
		gameObject->GetComponent<TextureComponent>()->SetTexture("Enemies/" + enemy + "Left.png", 0.1f, 2);
		gameObject->GetComponent<EntityMovementComponent>()->SetGhostModeEnabled(false);
		gameObject->GetComponent<EntityMovementComponent>()->DisableMovement(false);
		if (auto comp{ gameObject->GetComponent<EnemyComponent>() }) {
			comp->SetLifeState(MathLib::ELifeState::ALIVE);
		}
	}
}

void dae::MovingState::Update()
{
	if (m_IsVersus) return;

	m_Timer -= Timer::GetInstance().GetDeltaTime();
	if (m_Timer > 0) return;
	if (MathLib::CalculateChance() >= 80) {
		if (EnemyType == EEnemyType::Fygar) {
			if (MathLib::CalculateChance() >= 50) {
				gameObject->GetComponent<EnemyComponent>()->SetState(new BreatheFireState());
			}
			else {
				gameObject->GetComponent<EnemyComponent>()->SetState(new GhostState());
			}
		}
		else {
			gameObject->GetComponent<EnemyComponent>()->SetState(new GhostState());
		}
	}
}

void dae::InflatingState::Update()
{

}

void dae::GhostState::Init()
{
	gameObject->GetComponent<dae::AudioComponent>()->PlayGhostSound();

	auto players{ m_Scene->GetGameObjects(EnumStrings[Names::PlayerGeneral], false) };
	int randNr{ MathLib::CalculateChance(static_cast<int>(players.size()) - 1) };

	auto id{ m_Scene->GetGameObject(EnumStrings[Names::PlayerGeneral] + std::to_string(randNr))->GetComponent<EntityMovementComponent>()->GetCurrentTileId() };
	m_CachedLocation  = m_Scene->GetGameObject(std::to_string(id))->GetCenter();
	gameObject->GetComponent<EntityMovementComponent>()->SetGhostModeEnabled(true);
	gameObject->GetComponent<EntityMovementComponent>()->SetGhostLocation(m_CachedLocation);
	gameObject->GetComponent<EnemyComponent>()->SetLifeState(MathLib::ELifeState::INVINCIBLE);
}

void dae::GhostState::Update()
{
	//if reached a dug tile near a player
	float dx = m_CachedLocation.x - gameObject->GetCenter().x;
	float dy = m_CachedLocation.y - gameObject->GetCenter().y;
	float distanceToTarget = std::sqrt(dx * dx + dy * dy);
	if (distanceToTarget <= 1) {
		//gameObject->GetComponent<dae::AudioComponent>()->StopSound();
		gameObject->GetComponent<EnemyComponent>()->SetState(new MovingState());
	}
}

void dae::BreatheFireState::Init()
{
	gameObject->GetComponent<dae::AudioComponent>()->PlayFireSound();
	gameObject->GetComponent<EntityMovementComponent>()->DisableMovement(true);
	auto dir{ gameObject->GetComponent<EntityMovementComponent>()->GetLastDirection() };
	gameObject->GetComponent<TextureComponent>()->SetTexture("Enemies/FygarPrepare"+dir+".png", 0.2f, 3);	
	gameObject->GetComponent<EnemyComponent>()->SetLifeState(MathLib::ELifeState::ALIVE);
}

void dae::BreatheFireState::Update()
{
	m_PrepareTimer -= Timer::GetInstance().GetDeltaTime();
	if (!m_IsPrepareComplete) {
		if (m_PrepareTimer <= 0) {
			m_IsPrepareComplete = true;
			auto dir{ gameObject->GetComponent<EntityMovementComponent>()->GetLastDirection() };
			gameObject->GetComponent<TextureComponent>()->SetTexture("Enemies/Fygar"+ dir +".png", 0.2f, 2, true, false);

			auto go{ std::make_shared<GameObject>() };
			m_Scene->Add(go);
			fireObject = go.get();
			go->AddComponent(std::make_unique<FireComponent>(m_Scene, gameObject));
		}
	}

	if (fireObject) {
		if (fireObject->GetComponent<FireComponent>()->GetIsFireFinished()) {
			fireObject->MarkForDestroy();
			gameObject->GetComponent<EnemyComponent>()->SetState(new MovingState());
		}
		else {
			auto players{ m_Scene->GetGameObjects(EnumStrings[Names::PlayerGeneral], false) };
			for (const auto& player : players) {
				if (MathLib::IsOverlapping(fireObject->GetComponent<TextureComponent>()->GetRect(), player->GetComponent<TextureComponent>()->GetRect())) {
					auto comp{ player->GetComponent<PlayerComponent>() };
					comp->Respawn();
				}
			}
		}
	}
}
