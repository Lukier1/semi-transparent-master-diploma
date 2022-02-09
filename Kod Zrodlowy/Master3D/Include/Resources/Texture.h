#pragma once

#include "../../stdafx.h"

#include <assimp\scene.h>
#include "Resource.h"
#include "TextureType.h"


namespace Resources {
	class Texture : public Resource {

	public:
		Texture(std::string filename, TextureType type);
		Texture(std::string filXP, std::string filXN, std::string filYP, std::string filYN, std::string filZP, std::string filZN);
		Texture(const Texture& t);

		virtual ~Texture();

		TextureType getTextureType() const;
		GLuint getTexID() const;

		// Inherited via Resource
		virtual std::string GetFileName() const override;
		virtual void Release() override;

		//Zwraca wartoœæ która jest s³uzy do zapisanie do odpowiedniej zmiennej w shaderze
		int setActiveTexture();
	protected:
		Texture(TextureType type);
		GLuint mTex = 0;
		const TextureType mType;
		std::vector<std::vector<unsigned char>> mImagesData;
		std::string mFilenames;
		int W, H;

		void LoadFromFile(std::string filename, std::vector<unsigned char>& dest, FIBITMAP *& dib, GLint & inFormat, GLint & outFormat);

		void LoadSideTexture(GLenum side_target, std::string filename);

		//TODO Do usuniecia
		void LoadFromFile(std::string filename, FIBITMAP * &dib, GLint &inFormat, GLint &outFormat);

	};
}