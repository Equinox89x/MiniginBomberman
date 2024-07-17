#include "BombComponent.h"
#include "EntityMovementComponent.h"
#include "EnemyComponent.h"
#include "Timer.h"
#include "PathwayCreatorComponent.h"
#include "InputComponent.h"
#include "Renderer.h"
#include <AudioComponent.h>

void dae::BombComponent::Init()
{
}

void dae::BombComponent::StartBomb(glm::vec3 loc)
{
	GetGameObject()->GetTransform()->SetPosition(loc);
	GetGameObject()->GetComponent<AudioComponent>()->PlayBombSound();
}

void dae::BombComponent::Update()
{
	// TODO Animate fuse sequence 
	// TODO make state

		m_DestroyTimer -= Timer::GetInstance().GetDeltaTime();
		if (m_DestroyTimer <= 0) {
			// TODO trigger explosion & animation sequence
		}
}

void dae::BombComponent::Render() const
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