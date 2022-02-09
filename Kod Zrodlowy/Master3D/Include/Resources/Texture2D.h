#pragma once

#include "Texture.h"

namespace Resources {
	class Texture2D : public Texture {
	public:
		Texture2D(std::string filename, TextureType type);
		Texture2D(const Texture2D& t);
		~Texture2D();

		virtual std::string GetFileName() const override;
		virtual void Release() override;
	private:
		std::vector<unsigned char> mImageData;
		std::string mFilename;
		FIBITMAP * mDib = nullptr;



		unsigned int mWidth, mHeight;
	};
}