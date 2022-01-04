#pragma once
#include "../../stdafx.h"
#include "../Math/Vec.h"
#include "../Resources/ShaderProgram.h"
#include "../Resources/Texture.h"
#include "../Scene/SceneObject.h"
#include "../Viewport.h"

namespace Geometry {
	class Plane : public SceneObject {
	public:
		Plane();
		void Init();
		
		
		void Render(const Viewport & viewport, const Resources::ShaderProgram * program);

		//TODO Wydzielic do innej klasy czeœæ metod
		void SetSize(Vec2 size);
		Vec2 GetSize() const;
		void SetRotation(Vec3 rotation);
		Vec3 GetRotation() const;
		
		void setColor(Vec3 color);
		Vec3 getColor() const;

		void setAlpha(float alpha);
		float getAlpha() const;

		//Nie trzeba uzywac tekstur w tym momencie
//		void SetTexture(const Resources::Texture &texture);
//		const Resources::Texture& GetTexture() const;
	private:
		Vec3 mPosition;
		Vec3 mRotation;
		Vec2 mSize;
		Vec3 mColor;

		GLuint mVAO;
		float mAlpha;

//		Resources::Texture mTexture;

	};
}