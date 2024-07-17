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
		void SetTexture(MathLib::Movement movement, const std::string& filename, float animSpeed = 0.1f, int nrOfFrames = 1, bool resetAnim = true, bool canProgress = true);
		void SetTexture(TextureData& textureData);
		void RemoveTexture(MathLib::Movement movement);
		void SetPosition(const float x, const float y);
		void AddPosition(const float x, const float y);
		void SetWorldPosition(const float x, const float y);
		void Scale(const float x, const float y);
		void Rotate(const float angle);
		void SetIsVisible(bool isVisible) { m_IsVisible = isVisible; };

		//void SetNrOfFrames(int nrOfFrames) { NrOfFrames = nrOfFrames; };
		void SetNrOfFrames(int nrOfFrames) { m_Texture.NrOfFrames = nrOfFrames; };
		const SDL_Rect& GetRect() { HandleAnimation(); return m_Rect; };
		void SetOffset(glm::vec2 offset) { Offset = offset; };

		//void SetFrame(int frameNr) { CurrentFrame = frameNr; HandleAnimation(); };
		void SetFrame(int frameNr) { m_Texture.CurrentFrame = frameNr; HandleAnimation(); };

		void SetDirty() { m_needsUpdate = true; };
		void SetCanRotate( bool canRotate) { m_CanRotate = canRotate; };
		void SetRotationSpeed( float speed) { m_RotationSpeed = speed; };

	private:
		TextureData m_Texture{};
		//std::string FileName{""};
		std::map<MathLib::Movement, TextureData> FileNames{};
		//std::shared_ptr<Texture2D> m_pTexture{};
		bool m_needsUpdate{ false }, m_IsVisible{ true };
		float Angle{ 0 };
		//int NrOfFrames{ 1 };
		//int CurrentFrame{ 1 };
		//float DefaultAnimTime{ 0.1f };
		//float AnimTimer{ DefaultAnimTime };
		SDL_Rect m_Rect{};
		SDL_Rect m_SrcRect{};
		SDL_Rect m_DstRect{};
		glm::vec2 m_Position{};
		glm::vec2 m_Scale{  };

		glm::vec2 Offset{};

		bool m_CanRotate{ false };
		float m_RotationSpeed{ 10 };

		
		void HandleAnimation();
	};
}

