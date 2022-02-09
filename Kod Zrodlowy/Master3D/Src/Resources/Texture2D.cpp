#include "stdafx.h"

#include <Resources\Texture2D.h>
#include <Exceptions\MissingResourceException.h>
#include <Exceptions\UnknownImageFormat.h>
using namespace Resources;

Texture2D::Texture2D(std::string filename, TextureType type) : Texture(type) {

	GLint inFormat= GL_RGBA, outFormat = GL_BGRA;
	//LOADFROMFILE
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	fif = FreeImage_GetFileType(filename.c_str());

	mDib = FreeImage_Load(fif, filename.c_str());
	if (!mDib) {
		Logger::getLogger()->Error("Can't load " + filename);
		throw MissingResourceException("TEXTURE", filename);
	}

	mFilenames += filename + ";";

	unsigned char* data = FreeImage_GetBits(mDib);
	const unsigned int size = FreeImage_GetDIBSize(mDib);
	
	W = FreeImage_GetWidth(mDib);
	H = FreeImage_GetHeight(mDib);
	switch (FreeImage_GetBPP(mDib))
	{
	case 32:
		inFormat = GL_RGBA;
		outFormat = GL_BGRA;
		break;
	case 24:
		inFormat = GL_RGB;
		outFormat = GL_BGR;
		break;
	case 8:
		inFormat = GL_R;
		outFormat = GL_R;
		break;
	case 16:
	default:
		throw new UnknownImageFormat("Image bits: " + std::to_string(FreeImage_GetBPP(mDib)));
	}

	//###########

	mTex = 0;
	glGenTextures(1, &mTex);
	//Dobrac za pomoca parametru type

	setActiveTexture();

	glBindTexture(GL_TEXTURE_2D, mTex);

	glTexImage2D(
		GL_TEXTURE_2D, 0, inFormat,
		W, H, 0, outFormat, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//TODO Do nowej metody
	//Sparametryzowaæ
	// work out maximum an-isotropy
	GLfloat max_aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	// set the maximum!
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);

	//######################

	glGenerateMipmap(GL_TEXTURE_2D);

	Logger::getLogger()->Info("Loaded texture " + filename);


}
Texture2D::Texture2D(const Texture2D& t) : Texture(t.mType) {
	mTex = t.mTex;
	W = t.W;
	H = t.H;
}
Texture2D::~Texture2D() {
	FreeImage_Unload(mDib);
}

std::string Resources::Texture2D::GetFileName() const
{
	return mFilename;
}

void Resources::Texture2D::Release()
{

}
