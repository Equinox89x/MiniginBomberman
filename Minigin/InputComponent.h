#pragma once
#include <string>
#include <memory>
#include "TransformComponent.h"
#include "MathLib.h"
#include "Component.h"
#include <map>
#include "Scene.h"

namespace dae
{
	class InputComponent final : public Component
	{
	public:

		InputComponent(Scene* scene, glm::vec3 startPos, bool isController) : m_Scene{ scene }, m_StartPos{ startPos }, m_Movespeed{ 0,0,0 }, m_IsController{ isController } {}
		~InputComponent() {};
		InputComponent(const InputComponent& other) = delete;
		InputComponent(InputComponent&& other) = delete;
		InputComponent& operator=(const InputComponent& other) = delete;
		InputComponent& operator=(InputComponent&& other) = delete;

		void Update() override;
		virtual void FixedUpdate() override {};
		void Render() const override {};

		void SetMoveSpeed(const glm::vec3& movespeed);
		void SetMoveSpeed(const glm::vec3& movespeed, MathLib::Movement direction, bool isController);
		void StopMovement(MathLib::Movement direction);
		void SetCanMove(MathLib::Side side, bool canMove) {
			m_Movement[side] = canMove;
		};
		const std::map<MathLib::Side, bool>& GetCanMove() { return m_Movement; };
		void DisableMovement(bool IsDisabled) { m_CanMove = !IsDisabled; };

	private:
		Scene* m_Scene{ nullptr };
		glm::vec3 m_StartPos;
		glm::vec3 m_Movespeed;

		bool m_CanMove{ true }, m_IsController{false};

		std::map<MathLib::Movement, const glm::vec3> m_MoveSpeedList{};

		std::map<MathLib::Side, bool> m_Movement{
			  std::make_pair(MathLib::Side::Top, true),
			  std::make_pair(MathLib::Side::Left, true),
			  std::make_pair(MathLib::Side::Right, true),
			  std::make_pair(MathLib::Side::Bottom, true),
		};

		void UpdatePos(float dt);
	};
}