#include "PlayerComponent.h"
#include <ValuesComponent.h>
#include "MenuComponent.h"
#include "EntityMovementComponent.h"
#include <InputComponent.h>
#include "EnemyComponent.h"

dae::PlayerComponent::~PlayerComponent()
{
	delete m_PlayerState;
	m_PlayerState = nullptr;
}

void dae::PlayerComponent::Update()
{
	//float dt{ Timer::GetInstance().GetDeltaTime() };


	/*if (m_PlayerState == MathLib::RESPAWN) {
		m_InvincibilityTimer -= dt;
		if (m_InvincibilityTimer < 0) {
			m_PlayerState = MathLib::INVINCIBLE;
			GetGameObject()->GetComponent<TextureComponent>()->SetIsVisible(true);
			GetGameObject()->GetComponent<InputComponent>()->DisableMovement(false);
			GetGameObject()->GetComponent<EntityMovementComponent>()->DisableMovement(false);
			m_InvincibilityTimer = 3;
		}
	}
	if (m_PlayerState == MathLib::INVINCIBLE) {
		m_InvincibilityTimer -= dt;
		if (m_InvincibilityTimer < 0) {
			m_PlayerState = MathLib::ALIVE;
			m_InvincibilityTimer = 3;
		}
	}

	if (m_PlayerState == MathLib::ALIVE) {
		if (m_Scene) {
			auto enemies{ m_Scene->GetGameObjects(EnumStrings[Names::EnemyGeneral], false) };
			for (const auto& enemy : enemies) {
				if (enemy && !enemy->IsMarkedForDestroy()) {
					auto lifestate{ enemy->GetComponent<EnemyComponent>()->GetLifeState() };
					if (lifestate != MathLib::DEAD && lifestate != MathLib::INFLATING) {
						if (MathLib::IsOverlapping(GetGameObject()->GetComponent<TextureComponent>()->GetRect(), enemy->GetComponent<TextureComponent>()->GetRect())) {
							Respawn();
						}
					}
				}
			}
		}
	}*/

	m_PlayerState->Update();
}

void dae::PlayerComponent::Render() const
{
}

void dae::PlayerComponent::Init()
{
	SetState(new AliveState(), MathLib::ALIVE);
}

void dae::PlayerComponent::Reposition()
{
	GetGameObject()->GetTransform()->Translate(m_OriginalPosition);
}

void dae::PlayerComponent::Respawn()
{
	if (!GetGameObject()) return;
	if (GetGameObject()->IsMarkedForDestroy()) return;
	if (GetGameObject()->GetComponent<ValuesComponent>()->GetLives() > 0) {
		if (m_State == MathLib::ALIVE && m_State != MathLib::INVINCIBLE) {
			SetState(new RespawnState(), MathLib::RESPAWN);
		}
	}
	else {
		SetState(new DeadState(), MathLib::DEAD);
		GetGameObject()->GetComponent<ValuesComponent>()->Damage();
	}
}

void dae::AliveState::Update()
{
	if (m_Scene && gameObject && m_Ptr != reinterpret_cast<int*>(gameObject)) {
		if (gameObject->IsMarkedForDestroy()) return;
		auto enemies{ m_Scene->GetGameObjects(EnumStrings[Names::EnemyGeneral], false) };
		for (const auto& enemy : enemies) {
			if (enemy && !enemy->IsMarkedForDestroy()) {
				auto lifestate{ enemy->GetComponent<EnemyComponent>()->GetLifeState() };
				if (lifestate != MathLib::INFLATING && lifestate != MathLib::DEAD) {
					if (gameObject && m_Ptr != reinterpret_cast<int*>(gameObject) && !gameObject->IsMarkedForDestroy()) {
						if (MathLib::IsOverlapping(gameObject->GetComponent<TextureComponent>()->GetRect(), enemy->GetComponent<TextureComponent>()->GetRect())) {
							gameObject->GetComponent<PlayerComponent>()->Respawn();
						}
					}
				}
			}
		}
	}
}

void dae::InvincibleState::Update()
{
	float dt{ Timer::GetInstance().GetDeltaTime() };

	m_Timer -= dt;
	if (m_Timer < 0) {
		if (gameObject)gameObject->GetComponent<PlayerComponent>()->SetState(new AliveState(), MathLib::ALIVE);
	}
}

void dae::RespawnState::Init()
{
	if (!gameObject) return;
	gameObject->GetComponent<ValuesComponent>()->Damage();
	gameObject->GetComponent<PlayerComponent>()->Reposition();
	gameObject->GetComponent<TextureComponent>()->SetIsVisible(false);
	gameObject->GetComponent<InputComponent>()->DisableMovement(true);
	gameObject->GetComponent<EntityMovementComponent>()->DisableMovement(true);
}

void dae::RespawnState::Update()
{
	float dt{ Timer::GetInstance().GetDeltaTime() };

	m_Timer -= dt;
	if (m_Timer < 0) {
		if (!gameObject) return;

		gameObject->GetComponent<TextureComponent>()->SetIsVisible(true);
		gameObject->GetComponent<InputComponent>()->DisableMovement(false);
		gameObject->GetComponent<EntityMovementComponent>()->DisableMovement(false);
		gameObject->GetComponent<PlayerComponent>()->SetState(new InvincibleState(), MathLib::INVINCIBLE);
	}
}
