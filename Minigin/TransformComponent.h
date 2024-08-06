#pragma once
#include <glm/glm.hpp>
#include "GameObject.h"
#include "Component.h"

namespace dae {
	struct Transform final
	{
		Transform() : Transform{ 0.f, 0.f, 0.f, 0.f }
		{}
		Transform(float x, float y, float z, float r = 0.0f) : Position{ std::move(x), std::move(y), std::move(z) }
			, Rotation{ std::move(r) }
			, Scale{ 1.f, 1.f }
			, Forward{ 1.f, 0.f }
			, Right{ 0.f, -1.f }
		{}
		Transform(glm::vec2 vec2, float z, float r = 0.0f) : Transform{ std::move(vec2.x), std::move(vec2.y), std::move(z), std::move(r) }
		{}
		Transform(glm::vec3 vec3, float r = 0.0f) : Transform{ std::move(vec3.x), std::move(vec3.y), std::move(vec3.z), std::move(r) }
		{}

		glm::vec3 Position;
		glm::vec2 Scale;
		float Rotation;
		glm::vec2 Forward;
		glm::vec2 Right;
	};

	class TransformComponent final : public Component
	{
		using BYTE = unsigned char;
		enum TransformChange : BYTE
		{
			None = 0,
			Translation = 1,
			Rotation = 2,
			Scale = 4,
		};
	public:
		TransformComponent();

		~TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		TransformComponent(TransformComponent&& other) = default;
		TransformComponent& operator=(const TransformComponent& other) = default;
		TransformComponent& operator=(TransformComponent&& other) = default;

		void Init() override;
		void Render() const override{};
		void Update() override;

		const Transform& GetLocal() const { return m_Local; }
		const Transform& GetWorld() { Update(); return m_World; } //TODO: possible inaccuracies

		void Translate(float x, float y);
		void Translate(const glm::vec2& vec2);
		void Translate(const glm::vec3& vec3);
		void SetPosition(float x, float y, float z = 0.f);
		void SetPosition(const glm::vec2& vec2, float z = 0.f);
		void SetPosition(const glm::vec3& vec3);

		void Rotate(float r);
		void SetRotation(float r);

		void SetScale(float x, float y);
		void SetScale(const glm::vec2& scale);

	private:
		BYTE m_TransformChanges;
		Transform m_Local;
		Transform m_World;

		void CalculateWorldPosition();
		void CalculateWorldRotation();
		void CalculateWorldScale();

		void CalculateFwdRight();
	};
}
