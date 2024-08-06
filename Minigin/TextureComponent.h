#pragma once
#include "Component.h"
#include <string>
#include <memory>
#include "TransformComponent.h"
#include "Font.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Texture2D.h"
#include "MathLib.h"
#include "TransformComponent.h"
#include <map>


namespace dae {

	class Font;
	class Texture2D;
	class TextureComponent : public Component
	{
	public:
		TextureComponent() {};
		//TextureComponent(const std::string& filename, glm::vec2 position = { 0,0 }, glm::vec2 scale = { 1,1 }, const float rotation = 0, float animSpeed = 0.3f, int nrOfFrames = 1);
		virtual ~TextureComponent() override;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

		struct TextureData {
			std::string FileName{ "" };
			std::shared_ptr<Texture2D> m_pTexture{};
			int NrOfFrames{ 1 };
			int CurrentFrame{ 1 };
			float DefaultAnimTime{ 0.1f };
			float AnimTimer{ DefaultAnimTime };
			bool CanProgress{ true };

		};

		void Update() override;
		void Render() const;

		void SetTexture(const std::string& filename, float animSpeed = 0.1f, int nrOfFrames = 1, bool resetAnim = true, bool canProgress = true);
		void SetTexture(MathLib::EMovement movement, const std::string& filename, float animSpeed = 0.1f, int nrOfFrames = 1, bool resetAnim = true, bool canProgress = true);
		void SetTexture(const TextureData& textureData);
		void RemoveTexture(MathLib::EMovement movement);
		void SetIsVisible(bool isVisible) { m_IsVisible = isVisible; };
		void Rotate(const float angle);

		void SetNrOfFrames(int nrOfFrames) { m_Texture.NrOfFrames = nrOfFrames; };
		const SDL_Rect& GetRect() { HandleAnimation(); return m_Rect; };

		void SetFrame(int frameNr) { m_Texture.CurrentFrame = frameNr; HandleAnimation(); };

		void SetDirty() { m_needsUpdate = true; };
		void SetCanRotate( bool canRotate) { m_CanRotate = canRotate; };
		void SetRotationSpeed( float speed) { m_RotationSpeed = speed; };

	private:
		TextureData m_Texture{};
		std::map<MathLib::EMovement, TextureData> FileNames{};
		bool m_needsUpdate{ false }, m_IsVisible{ true };
		float Angle{ 0 };
		SDL_Rect m_Rect{};
		SDL_Rect m_SrcRect{};
		SDL_Rect m_DstRect{};
		glm::vec2 m_Position{};
		glm::vec2 m_Scale{  };


		bool m_CanRotate{ false };
		float m_RotationSpeed{ 10 };

		
		void HandleAnimation();
	};
}

