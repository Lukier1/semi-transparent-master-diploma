#pragma once

#pragma once
#include "../../stdafx.h"
#include "../Math/Vec.h"
#include "../Resources/ShaderProgram.h"
#include "../Resources/Texture.h"
#include "../Scene/SceneObject.h"
#include "../Viewport.h"

namespace Particles {
	class AlphaParticle : public SceneObject {
	public:
		AlphaParticle(Resources::Texture &texture);
		void Init();

		void RenderMulti(const Matrix4x4& perspMat, const Matrix4x4& viewMat, const Resources::ShaderProgram * program);

		void Render(const Matrix4x4& perspMat, const Matrix4x4& viewMat, const Resources::ShaderProgram* program);

		void Render(const Viewport & viewport, const Resources::ShaderProgram * program);

		//TODO Wydzielic do innej klasy czeœæ metod
		void SetSize(Vec2 size);
		Vec2 GetSize() const;
		void SetRotation(Vec3 rotation);
		Vec3 GetRotation() const;

		void setAlpha(float alpha);
		float getAlpha() const;

		//Nie trzeba uzywac tekstur w tym momencie
		void SetTexture(const Resources::Texture &texture);
		const Resources::Texture& GetTexture() const;
	private:
		Vec3 mRotation;
		Vec2 mSize;
		Vec3 mColor;

		GLuint mVAO;
		float mAlpha;

		Resources::Texture mTexture;
	};
}