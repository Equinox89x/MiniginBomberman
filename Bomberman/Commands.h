#pragma once
#include "../Bomberman/BombComponent.h"
#include "../Bomberman/EnemyComponent.h"
#include "../Bomberman/EntityMovementComponent.h"
#include "../Bomberman/HighscoreComponent.h"
#include "../Bomberman/MenuComponent.h"
#include "../Bomberman/PlayerComponent.h"
#include "AudioComponent.h"
#include "Command.h"
#include "GameObject.h"
#include "InputComponent.h"
#include "MathLib.h"
#include "TextureComponent.h"
#include "ValuesComponent.h"
#include <functional>

namespace dae
{

#pragma region EMovement
	class Move final : public Command
	{
	public:
		Move(Scene* scene, dae::GameObject* object, MathLib::EMovement movement, std::string textureName, const glm::vec3& moveSpeed) : m_pObject(object), m_Movement{ movement }, m_MoveSpeed(moveSpeed), m_TextureName(textureName), m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive())
				return;
			auto player{ m_pObject->GetComponent<dae::EntityMovementComponent>() };
			if (!player)
				return;

			auto input{ m_pObject->GetComponent<dae::InputComponent>() };
			auto tex{ m_pObject->GetComponent<dae::TextureComponent>() };

			if (auto playerComp{ m_pObject->GetComponent<PlayerComponent>() })
			{
				auto state{ playerComp->GetState() };
				if (state != MathLib::ELifeState::ALIVE && state != MathLib::ELifeState::INVINCIBLE)
					return;
			}

			player->SetMovement(m_Movement);
			input->SetMoveSpeed(m_MoveSpeed, m_Movement, false);
			tex->SetTexture(m_Movement, m_TextureName + ".png", 0.1f, 3);
		}
		void Execute(glm::vec2 pos) override
		{
			if (!m_Scene->GetIsActive())
				return;
			auto player{ m_pObject->GetComponent<dae::EntityMovementComponent>() };
			if (!player)
				return;

			m_MoveSpeed = glm::vec3{ pos.x * 100, pos.y * -100, 0 };
			auto input{ m_pObject->GetComponent<dae::InputComponent>() };
			auto tex{ m_pObject->GetComponent<dae::TextureComponent>() };

			if (auto playerComp{ m_pObject->GetComponent<PlayerComponent>() })
			{
				if (pos.x > 0)
				{
					m_Movement = MathLib::EMovement::RIGHT;
					m_TextureName = "Character/moveRight.png";
				}
				else if (pos.x < 0)
				{
					m_Movement = MathLib::EMovement::LEFT;
					m_TextureName = "Character/moveLeft.png";
				}
				else if (pos.y > 0)
				{
					m_Movement = MathLib::EMovement::UP;
					m_TextureName = "Character/moveUp.png";
				}
				else if (pos.y < 0)
				{
					m_Movement = MathLib::EMovement::DOWN;
					m_TextureName = "Character/moveDown.png";
				}

				auto state{ playerComp->GetState() };
				if (state != MathLib::ELifeState::ALIVE && state != MathLib::ELifeState::INVINCIBLE)
					return;
			}

			player->SetMovement(m_Movement);
			input->SetMoveSpeed(m_MoveSpeed, m_Movement, true);
			tex->SetTexture(m_Movement, m_TextureName, 0.1f, 3);
		};

	private:
		Scene* m_Scene{ nullptr };

		dae::GameObject*   m_pObject;
		glm::vec3		   m_MoveSpeed;
		std::string		   m_TextureName;
		MathLib::EMovement m_Movement;
	};

	class StopMove final : public Command
	{
	public:
		StopMove(Scene* scene, dae::GameObject* object, MathLib::EMovement direction) : m_pObject(object), m_Movement{ direction }, m_Scene{ scene } {};
		void Execute() override
		{
			if (!m_Scene->GetIsActive())
				return;

			if (auto comp{ m_pObject->GetComponent<dae::InputComponent>() })
				comp->StopMovement(m_Movement);
			if (auto comp{ m_pObject->GetComponent<dae::TextureComponent>() })
				comp->RemoveTexture(m_Movement);
		}
		void Execute(glm::vec2) override{};

	private:
		Scene*			   m_Scene{ nullptr };
		dae::GameObject*   m_pObject;
		MathLib::EMovement m_Movement;
	};
#pragma endregion

#pragma region values
	class IncreaseScore final : public Command
	{
	public:
		IncreaseScore(Scene* scene, GameObject* object) : m_pObject(object), m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive())
				return;

			m_pObject->GetComponent<ValuesComponent>()->IncreaseScore(100);
		}
		void Execute(glm::vec2) override{};

	private:
		Scene* m_Scene{ nullptr };

		GameObject* m_pObject;
	};
#pragma endregion

	class StartBomb final : public Command
	{
	public:
		StartBomb(Scene* scene, GameObject* const object) : m_pObject(object), m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_CanDrop)
			{
				m_CanDrop = true;
				return;
			}
			if (!m_Scene->GetIsActive())
				return;

			if (!m_pObject)
				return;
			if (auto comp{ m_pObject->GetComponent<EntityMovementComponent>() })
			{
				auto bombs{ m_Scene->GetGameObjects(EnumStrings[Names::Bomb]) };
				std::vector<GameObject*> playerBombs{};
				std::copy_if(bombs.begin(), bombs.end(), std::back_inserter(playerBombs),
							 [&](GameObject* go)
							 {
								 auto bombComp{ go->GetComponent<BombComponent>() };
								 return bombComp->BombDropper->GetName() == m_pObject->GetName();
							 });

				if (auto playerComp{ m_pObject->GetComponent<PlayerComponent>() })
				{
					if (static_cast<int>(playerBombs.size()) >= playerComp->GetMaxBombs())
						return;

					auto pBombObject = std::make_unique<GameObject>();
					pBombObject->SetName(EnumStrings[Names::Bomb]);
					int tileId{ comp->GetCurrentTileId() };
					pBombObject->AddComponent(std::make_unique<dae::BombComponent>(m_Scene, m_pObject))->StartBomb(tileId, playerComp->GetBombStrength());
					m_Scene->Add(std::move(pBombObject));
				}
			}
		}
		void Execute(glm::vec2) override{};

	private:
		Scene*		m_Scene{ nullptr };
		GameObject* m_pObject;
		bool		m_CanDrop{ false };
	};

	class Detonate final : public Command
	{
	public:
		Detonate(Scene* scene, GameObject* const object) : m_pObject(object), m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive())
				return;

			if (!m_pObject)
				return;

			if (auto playerComp{ m_pObject->GetComponent<PlayerComponent>() })
			{
				if (playerComp->HasDetonator)
				{
					if (auto gameObject{ playerComp->GetOldestBomb(m_pObject->GetName()) })
					{
						auto bomb(gameObject->GetComponent<BombComponent>());
						bomb->SetState(new ExplosionState(m_Scene), MathLib::EBombState::Explosion);
					}
				}
			}
		}
		void Execute(glm::vec2) override{};

	private:
		Scene*		m_Scene{ nullptr };
		GameObject* m_pObject;
	};

#pragma region Menu
	class CycleGameMode final : public Command
	{
	public:
		CycleGameMode(Scene* scene, GameObject* object, bool isMoveUp) : m_pObject(object), m_IsMoveUp{ isMoveUp }, m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive())
				return;

			m_pObject->GetComponent<MenuComponent>()->CycleGameMode(m_IsMoveUp);
		}
		void Execute(glm::vec2 pos) override
		{
			if (!m_Scene->GetIsActive())
				return;

			if (pos.y > 0)
			{
				m_pObject->GetComponent<MenuComponent>()->CycleGameMode(true);
			}
			else if (pos.y < 0)
			{
				m_pObject->GetComponent<MenuComponent>()->CycleGameMode(false);
			}
		};

	private:
		Scene* m_Scene{ nullptr };

		GameObject* m_pObject;
		bool		m_IsMoveUp{};
	};

	class StartGame final : public Command
	{
	public:
		StartGame(Scene* scene, GameObject* object) : m_pObject(object), m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive())
				return;

			m_pObject->GetComponent<MenuComponent>()->StartGame(m_pObject->GetParent());
		}
		void Execute(glm::vec2) override{};

	private:
		Scene* m_Scene{ nullptr };

		GameObject* m_pObject;
	};

#pragma endregion
#pragma region Skipping
	class Skip final : public Command
	{
	public:
		Skip(Scene* scene, GameObject* object) : m_pObject(object), m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive())
				return;

			m_pObject->GetComponent<MenuComponent>()->SkipLevel();
		}
		void Execute(glm::vec2) override{};

	private:
		Scene*		m_Scene;
		GameObject* m_pObject;
	};
#pragma endregion

#pragma region Highscore
	class MoveKeyboardCursor final : public Command
	{
	public:
		MoveKeyboardCursor(Scene* scene, GameObject* object, glm::vec2 key) : m_pObject(object), m_Scene{ scene }, m_Key{ key } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive())
				return;
			m_pObject->GetComponent<HighscoreComponent>()->MoveCursor(m_Key);
		}
		void Execute(glm::vec2) override
		{
			if (!m_Scene->GetIsActive())
				return;
			m_pObject->GetComponent<HighscoreComponent>()->MoveCursor(m_Key);
		};

	private:
		Scene*		m_Scene;
		GameObject* m_pObject;
		glm::vec2	m_Key{};
	};

	class SelectKey final : public Command
	{
	public:
		SelectKey(Scene* scene, GameObject* object) : m_pObject(object), m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive())
				return;
			m_pObject->GetComponent<HighscoreComponent>()->Select();
		}
		void Execute(glm::vec2) override{};

	private:
		Scene*		m_Scene;
		GameObject* m_pObject;
	};

	class BackToMenu final : public Command
	{
	public:
		BackToMenu(Scene* scene, std::function<void()> makeMenuFn) : m_MakeMenuFn(makeMenuFn), m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive())
				return;
			auto scene{ SceneManager::GetInstance().GetActiveScene() };
			scene->SetActive(false);
			m_MakeMenuFn();
			SceneManager::GetInstance().DeleteScene(scene);
		}
		void Execute(glm::vec2) override{};

	private:
		Scene*				  m_Scene;
		std::function<void()> m_MakeMenuFn;
	};
#pragma endregion

} // namespace dae