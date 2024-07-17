#pragma once
#include "Component.h"
#include "GameObject.h"

namespace dae
{
	class RotatorComponent final : public Component
	{
	public:
		RotatorComponent();
		virtual ~RotatorComponent() = default;
		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;
		RotatorComponent& operator=(const RotatorComponent& other) = delete;
		RotatorComponent& operator=(RotatorComponent&& other) = delete;

		void Update( ) override;

		void SetRotationSpeed(float speed) { m_Speed = speed; };

	private:

		float m_Angle{ 0 };
		float m_Speed{ 10 };
	};
}