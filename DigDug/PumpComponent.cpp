#include "PumpComponent.h"
#include "EntityMovementComponent.h"
#include "EnemyComponent.h"
#include "Timer.h"
#include "PathwayCreatorComponent.h"
#include "InputComponent.h"
#include "Renderer.h"

void dae::PumpComponent::Init()
{
}

void dae::PumpComponent::Pump(glm::vec3 loc)
{
	//On space press, we shoot, or pump if we have an enemy attached
	m_DestroyTimer = 0.7f;

	if (m_SelectedEnemy) {
		//Stop pump movement
		//m_CanMove = false;
		//m_SelectedEnemy->GetComponent<dae::EntityMovementComponent>()->DisableMovement(true);
		//release if enemy dead
		if (m_SelectedEnemy->IsMarkedForDestroy()) {
			ReleasePump();
		}
		else if (m_SelectedEnemy->GetComponent<EnemyComponent>()->PumpUp()) {
			ReleasePump();
		}
	}
	else {
		//Start moving the pump
		m_CanMove = true;
		ResetMovement(loc);
		GetGameObject()->GetComponent<TextureComponent>()->SetIsVisible(true);
	}
}

void dae::PumpComponent::ResetMovement(glm::vec3 /*loc*/)
{
	auto texComp2 = m_Player->GetComponent<TextureComponent>()->GetRect();
	m_StartPos.x = static_cast<float>(texComp2.x + texComp2.w / 2);
	m_StartPos.y = static_cast<float>(texComp2.y + texComp2.h / 2);
	auto dir = m_Player->GetComponent<EntityMovementComponent>()->GetDirection();
		switch (dir)
		{
		case MathLib::UP:
			m_StartPos.x -= texComp2.w /3;
				//GetGameObject()->GetTransform()->Translate(0, -speed);
				GetGameObject()->GetComponent<TextureComponent>()->SetTexture("Character/PumpUp.png", 0.1f, 1);

			break;
		case MathLib::DOWN:
			m_StartPos.x -= texComp2.w / 3;
			//GetGameObject()->GetTransform()->Translate(0, speed);
			GetGameObject()->GetComponent<TextureComponent>()->SetTexture("Character/PumpDown.png", 0.1f, 1);

			break;
		case MathLib::LEFT:
			m_StartPos.y -= texComp2.h / 3;
			//GetGameObject()->GetTransform()->Translate(-speed, 0);
			GetGameObject()->GetComponent<TextureComponent>()->SetTexture("Character/PumpLeft.png", 0.1f, 1);

			break;
		case MathLib::RIGHT:
			m_StartPos.y -= texComp2.h / 3;
			//GetGameObject()->GetTransform()->Translate(speed, 0);
			GetGameObject()->GetComponent<TextureComponent>()->SetTexture("Character/PumpRight.png", 0.1f, 1);

			break;
		default:
			break;
		}
	//m_StartPos = loc;
	GetGameObject()->GetTransform()->Translate(m_StartPos);
}

void dae::PumpComponent::ReleasePump()
{
	if (IsMarkedForDestroy()) return;
	if (m_Player) {
		m_CanMove = false;
		if (auto comp{ m_Player->GetComponent<dae::EntityMovementComponent>() }) comp->DisableMovement(false);
		if (auto comp{ m_Player->GetComponent<dae::InputComponent>() }) comp->DisableMovement(false);
		m_SelectedEnemy = nullptr;
		ResetMovement({0,0,0});
		if (auto go{ GetGameObject() }) go->GetComponent<TextureComponent>()->SetIsVisible(false);
	}
}

void dae::PumpComponent::AttachPump(GameObject* enemy)
{
	m_SelectedEnemy = enemy;
	m_SelectedEnemy->GetComponent<EnemyComponent>()->SetPlayer(m_Player);
	auto comp{ m_SelectedEnemy->GetComponent<dae::EntityMovementComponent>() };

	auto dir{ m_SelectedEnemy->GetComponent<EntityMovementComponent>()->GetLastDirection() };
	m_SelectedEnemy->GetComponent<TextureComponent>()->SetTexture("Enemies/"+ comp->GetEnemyName() + "Inflate"+ dir +".png", 0.f, 4, true, false);
	comp->DisableMovement(true);
	m_CanMove = false;
	m_Player->GetComponent<dae::EntityMovementComponent>()->DisableMovement(true);
	m_Player->GetComponent<dae::InputComponent>()->DisableMovement(true);
}

void dae::PumpComponent::SetPlayer(GameObject* player)
{
	m_Player = player;
}

void dae::PumpComponent::Update()
{
	auto texComp = GetGameObject()->GetComponent<TextureComponent>();
	auto texComp2 = m_Player->GetComponent<TextureComponent>()->GetRect();
	auto rect = texComp->GetRect();

	UpdatePosition();

	if (!m_SelectedEnemy) {
		auto enemies{ m_Scene->GetGameObjects(EnumStrings[Names::EnemyGeneral], false) };
		if (enemies.size() > 0) {
			for (auto enemy : enemies) {
				if (MathLib::IsOverlapping(enemy->GetComponent<TextureComponent>()->GetRect(), rect)) {
					if (enemy->GetComponent<EnemyComponent>()->GetLifeState() != MathLib::ELifeState::INVINCIBLE) {
						AttachPump(enemy.get());
					}
				}
			}
		}
		m_DestroyTimer -= Timer::GetInstance().GetDeltaTime();
		if (m_DestroyTimer <= 0) {
			ReleasePump();
		}
	}


	auto paths{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator]) };
	auto pathcomp{ paths->GetComponent<PathwayCreatorComponent>() };
	for (auto path : pathcomp->GetPathways()) {
		//Release Pump if it hits a wall;
		if (MathLib::IsOverlapping(path.second.TextureComponent->GetRect(), rect)) {
			if (path.second.PathState == EPathState::Blocker) {
				ReleasePump();
			}
		}
	}
}

void dae::PumpComponent::Render() const
{
	//auto texComp = GetGameObject()->GetComponent<TextureComponent>();
	//auto texComp2 = m_Player->GetComponent<TextureComponent>()->GetRect();
	//auto rect = texComp->GetRect();
	//rect.w /= 3;
	//rect.h /= 3;
	//rect.x = texComp2.x + texComp2.w / 2;
	//rect.y = texComp2.y + texComp2.h / 2;

	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255); // Set the color to red
	//SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &rect); // D
}


void dae::PumpComponent::UpdatePosition()
{
	if (!m_CanMove) return;
	auto dt = Timer::GetInstance().GetDeltaTime();
	auto speed = dt * 200.f;
	auto dir = m_Player->GetComponent<EntityMovementComponent>()->GetDirection();
	switch (dir)
	{
	case MathLib::UP:
		GetGameObject()->GetTransform()->Translate(0, -speed);
		//GetGameObject()->GetComponent<TextureComponent>()->SetTexture("Character/PumpUp.png", 0.1f, 1);

		break;
	case MathLib::DOWN:
		GetGameObject()->GetTransform()->Translate(0, speed);
		//GetGameObject()->GetComponent<TextureComponent>()->SetTexture("Character/PumpDown.png", 0.1f, 1);

		break;
	case MathLib::LEFT:
		GetGameObject()->GetTransform()->Translate(-speed, 0);
		//GetGameObject()->GetComponent<TextureComponent>()->SetTexture("Character/PumpLeft.png", 0.1f, 1);

		break;
	case MathLib::RIGHT:
		GetGameObject()->GetTransform()->Translate(speed, 0);
		//GetGameObject()->GetComponent<TextureComponent>()->SetTexture("Character/PumpRight.png", 0.1f, 1);

		break;
	default:
		break;
	}
}