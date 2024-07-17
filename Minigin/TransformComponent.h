#pragma once
#include <glm/glm.hpp>
#include "GameObject.h"
#include "Component.h"

namespace dae {
	class TransformComponent final : public Component
	{

	public:

		TransformComponent();
		virtual ~TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;

		void Update( ) override;
		//virtual void FixedUpdate() override;
		//void Render() const;

		void Translate(float x, float y, float z);
		void Translate(float x, float y);
		void Translate(glm::vec3 pos);
		void Translate(glm::vec2 pos);
		void TranslateWorld(float x, float y, float z);
		void TranslateWorld(float x, float y);
		void TranslateWorld(glm::vec3 pos);
		void TranslateWorld(glm::vec2 pos);

		void SetScale(float x, float y, float z);
		void SetScale(float x, float y);
		void SetScale(glm::vec3 pos);
		void SetScale(glm::vec2 pos);
		void SetScale(float scale);

		void AddTranslate(float x, float y, float z);
		void AddTranslate(float x, float y);
		void AddTranslate(glm::vec3 pos);
		void AddTranslate(glm::vec2 pos);
		void AddTranslateWorld(float x, float y, float z);
		void AddTranslateWorld(float x, float y);
		void AddTranslateWorld(glm::vec3 pos);
		void AddTranslateWorld(glm::vec2 pos);

		void Rotate(float x, float y, float z);
		void Rotate(const float angle);
		const glm::vec3& GetPosition() const { return m_Position; };
		const glm::vec3 GetFullPosition() const;
		const glm::vec3 GetWorldPosition();
		const glm::vec3& GetRotation() const { return m_Rotation; };
		const glm::vec3 GetWorldRotation() const;
		const glm::vec3 GetScale() const { return m_Scale; };

		glm::vec3 Rotate(const float cx, const float cy, float angle, glm::vec3 point, bool isDegrees);

		void SetRotationOffset(glm::vec3 offset) { m_Offset = offset; };

		void UpdateTransforms();
		bool IsDirty() const { return m_IsDirty; }
		void SetDirty();

	protected:
		bool CheckIfDirty();

	private:
		glm::vec3 m_Position{}, m_WorldPosition{}, m_Scale{ 1,1,1 }, m_Offset;
		glm::vec3 m_Rotation{}, m_WorldRotation{};
		bool m_IsDirty{};

	};
}
