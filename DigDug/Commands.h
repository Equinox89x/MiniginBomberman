#pragma once
#include "GameObject.h"
#include <functional>
#include "InputComponent.h"
#include "TextureComponent.h"
#include "../DigDug/EntityMovementComponent.h"
#include "MathLib.h"
#include "Command.h"
#include "ValuesComponent.h"
#include "AudioComponent.h"
#include "../DigDug/PumpComponent.h"
#include "../DigDug/MenuComponent.h"
#include "../DigDug/HighscoreComponent.h"
#include "../DigDug/PlayerComponent.h"
#include "../DigDug/EnemyComponent.h"

namespace dae {

	#pragma region Movement
	class Move final : public Command
	{
	public:
		Move(Scene* scene, dae::GameObject* object, MathLib::Movement movement, std::string textureName, const glm::vec3& moveSpeed) : 
			m_pObject(object), m_Movement{ movement }, m_MoveSpeed(moveSpeed), m_TextureName(textureName), m_Scene{scene} {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive()) return;
			auto player{ m_pObject->GetComponent<dae::EntityMovementComponent>() };

			if (auto playerComp{ m_pObject->GetComponent<PlayerComponent>() }) {
				auto state{ playerComp->GetState() };
				if (state == MathLib::ELifeState::ALIVE || state == MathLib::ELifeState::INVINCIBLE) {
					auto input{ m_pObject->GetComponent<dae::InputComponent>() };
					auto tex{ m_pObject->GetComponent<dae::TextureComponent>() };

					player->SetMovement(m_Movement);
					input->SetMoveSpeed(m_MoveSpeed, m_Movement, false);
					tex->SetTexture(m_Movement, m_TextureName + (player->GetShouldDig() ? "Dig.png" : ".png"), 0.1f, 2);
				}
			}
			else {
				auto input{ m_pObject->GetComponent<dae::InputComponent>() };
				auto tex{ m_pObject->GetComponent<dae::TextureComponent>() };

				if (m_Movement == MathLib::Movement::LEFT) {
					player->SetLastDirection("Left");
				}
				else if (m_Movement == MathLib::Movement::RIGHT) {
					player->SetLastDirection("Right");
				}
				player->SetMovement(m_Movement);
				input->SetMoveSpeed(m_MoveSpeed, m_Movement, false);
				tex->SetTexture(m_Movement, m_TextureName + ".png", 0.1f, 2);
			}
			
		}
		void Execute(glm::vec2 pos) override {
			if (!m_Scene->GetIsActive()) return;
			auto player{ m_pObject->GetComponent<dae::EntityMovementComponent>() };

			m_MoveSpeed = glm::vec3{ pos.x * 100,pos.y * -100, 0 };
			if (auto playerComp{ m_pObject->GetComponent<PlayerComponent>() }) {
				if (pos.x > 0) {
					m_Movement = MathLib::RIGHT;
					m_TextureName = "Character/moveRight";
				}
				else if (pos.x < 0) {
					m_Movement = MathLib::LEFT;
					m_TextureName = "Character/moveLeft";
				}
				else if (pos.y > 0) {
					m_Movement = MathLib::UP;
					m_TextureName = "Character/moveUp";
				}
				else if (pos.y < 0) {
					m_Movement = MathLib::DOWN;
					m_TextureName = "Character/moveDown";
				}

				auto state{ playerComp->GetState() };
				if (state == MathLib::ELifeState::ALIVE || state == MathLib::ELifeState::INVINCIBLE) {
					auto input{ m_pObject->GetComponent<dae::InputComponent>() };
					auto tex{ m_pObject->GetComponent<dae::TextureComponent>() };

					player->SetMovement(m_Movement);
					input->SetMoveSpeed(m_MoveSpeed, m_Movement, true);
					tex->SetTexture(m_Movement, m_TextureName + (player->GetShouldDig() ? "Dig.png" : ".png"), 0.1f, 2);
				}
			}
			else {
				if (pos.x > 0) {
					m_Movement = MathLib::RIGHT;
					m_TextureName = "Enemies/FygarRight";
				}
				else if (pos.x < 0) {
					m_Movement = MathLib::LEFT;
					m_TextureName = "Enemies/FygarLeft";
				}
				else if (pos.y > 0) {
					m_Movement = MathLib::UP;
					m_TextureName = "Enemies/FygarRight";
				}
				else if (pos.y < 0) {
					m_Movement = MathLib::DOWN;
					m_TextureName = "Enemies/FygarLeft";
				}
				auto input{ m_pObject->GetComponent<dae::InputComponent>() };
				auto tex{ m_pObject->GetComponent<dae::TextureComponent>() };

				if (m_Movement == MathLib::Movement::LEFT) {
					player->SetLastDirection("Left");
				}
				else if (m_Movement == MathLib::Movement::RIGHT) {
					player->SetLastDirection("Right");
				}
				player->SetMovement(m_Movement);
				input->SetMoveSpeed(m_MoveSpeed, m_Movement, true);
				tex->SetTexture(m_Movement, m_TextureName + ".png", 0.1f, 2);
			}
		};

	private:
		Scene* m_Scene{ nullptr };

		dae::GameObject* m_pObject;
		glm::vec3 m_MoveSpeed;
		std::string m_TextureName;
		MathLib::Movement m_Movement;
	};

	class StopMove final : public Command
	{
	public:
		StopMove(Scene* scene, dae::GameObject* object, MathLib::Movement direction) : m_pObject(object), m_Movement{ direction }, m_Scene{ scene } {};
		void Execute() override
		{
			if (!m_Scene->GetIsActive()) return;

			m_pObject->GetComponent<dae::InputComponent>()->StopMovement(m_Movement);
			m_pObject->GetComponent<dae::TextureComponent>()->RemoveTexture(m_Movement);
		}
		void Execute(glm::vec2) override {};

	private:
		Scene* m_Scene{ nullptr };
		dae::GameObject* m_pObject;
		MathLib::Movement m_Movement;
	};
	#pragma endregion

	#pragma region values
	class IncreaseScore final : public Command
	{
	public:
		IncreaseScore(Scene* scene, GameObject* object) : m_pObject(object), m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive()) return;

			m_pObject->GetComponent<ValuesComponent>()->IncreaseScore(100);
		}
		void Execute(glm::vec2) override {};

	private:
		Scene* m_Scene{ nullptr };

		GameObject* m_pObject;
	};


	class Pump final : public Command
	{
	public:
		Pump(Scene* scene, GameObject* const object, GameObject* const pumpObject) : m_pObject(object), m_pPumpObject{ pumpObject }, m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive()) return;

			if (!m_pObject && !m_pPumpObject) return;
			m_pObject->GetComponent<dae::AudioComponent>()->PlayPumpSound();
			m_pObject->GetComponent<dae::EntityMovementComponent>()->DisableMovement(true);
			m_pObject->GetComponent<dae::InputComponent>()->DisableMovement(true);
			m_pPumpObject->GetComponent<dae::PumpComponent>()->Pump(m_pObject->GetTransform()->GetFullPosition());
		}
		void Execute(glm::vec2) override {};

	private:
		Scene* m_Scene{ nullptr };

		GameObject* m_pObject;
		GameObject* m_pPumpObject;
	};

	class FygarFire final : public Command
	{
	public:
		FygarFire(Scene* scene, GameObject* const object) : m_pObject(object), m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive()) return;

			if (!m_pObject) return;
			m_pObject->GetComponent<EnemyComponent>()->SetState(new BreatheFireState());
		}
		void Execute(glm::vec2) override {};

	private:
		Scene* m_Scene{ nullptr };

		GameObject* m_pObject;
	};
	#pragma endregion

#pragma region Menu
	class CycleGameMode final : public Command
	{
	public:
		CycleGameMode(Scene* scene, GameObject* object, bool isMoveUp) : m_pObject(object), m_IsMoveUp{ isMoveUp }, m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive()) return;

			m_pObject->GetComponent<MenuComponent>()->CycleGameMode(m_IsMoveUp);
		}
		void Execute(glm::vec2 pos) override {
			if (!m_Scene->GetIsActive()) return;

			if (pos.y > 0) {
				m_pObject->GetComponent<MenuComponent>()->CycleGameMode(true);
			}
			else if(pos.y < 0) {
				m_pObject->GetComponent<MenuComponent>()->CycleGameMode(false);
			}
		};

	private:
		Scene* m_Scene{ nullptr };

		GameObject* m_pObject;
		bool m_IsMoveUp{};
	};

	class StartGame final : public Command
	{
	public:
		StartGame(Scene* scene, GameObject* object) : m_pObject(object), m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive()) return;

			m_pObject->GetComponent<MenuComponent>()->StartGame(m_pObject->GetParent());
		}
		void Execute(glm::vec2) override {};

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
			if (!m_Scene->GetIsActive()) return;

			m_pObject->GetComponent<MenuComponent>()->SkipLevel();
		}
		void Execute(glm::vec2) override {};

	private:
		Scene* m_Scene;
		GameObject* m_pObject;

	};
#pragma endregion
		
#pragma region Highscore
	class MoveKeyboardCursor final : public Command
	{
	public:
		MoveKeyboardCursor(Scene* scene, GameObject* object, glm::vec2 key) : m_pObject(object), m_Scene{ scene }, m_Key{key} {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive()) return;
			m_pObject->GetComponent<HighscoreComponent>()->MoveCursor(m_Key);
		}
		void Execute(glm::vec2) override {
			if (!m_Scene->GetIsActive()) return;
			m_pObject->GetComponent<HighscoreComponent>()->MoveCursor(m_Key);
		};

	private:
		Scene* m_Scene;
		GameObject* m_pObject;
		glm::vec2 m_Key{};

	};

	class SelectKey final : public Command
	{
	public:
		SelectKey(Scene* scene, GameObject* object) : m_pObject(object), m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive()) return;
			m_pObject->GetComponent<HighscoreComponent>()->Select();
		}
		void Execute(glm::vec2) override {};

	private:
		Scene* m_Scene;
		GameObject* m_pObject;

	};

	class BackToMenu final : public Command
	{
	public:
		BackToMenu(Scene* scene, std::function<void()> makeMenuFn) : m_MakeMenuFn(makeMenuFn), m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive()) return;
			auto scene{ SceneManager::GetInstance().GetActiveScene() };
			scene->SetActive(false);
			m_MakeMenuFn();
			SceneManager::GetInstance().DeleteScene(scene);
		}
		void Execute(glm::vec2) override {};

	private:
		Scene* m_Scene;
		std::function<void()> m_MakeMenuFn;

	};
#pragma endregion

}