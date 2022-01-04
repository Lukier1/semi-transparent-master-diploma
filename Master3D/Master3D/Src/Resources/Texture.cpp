
#include "stdafx.h"
#include <Resources/Texture.h>
#include <Exceptions\MissingResourceException.h>


using namespace Resources;

Texture::Texture(std::string filename, TextureType type) : mType(type)
{
	FIBITMAP * dib = nullptr;
	GLint inFormat, outFormat;
	LoadFromFile(filename, dib, inFormat, outFormat);

	mTex = 0;

	glGenTextures(1, &mTex);
	//Dobrac za pomoca parametru type

	setActiveTexture();
	glBindTexture(GL_TEXTURE_2D, mTex);

	glTexImage2D(
		GL_TEXTURE_2D, 0, inFormat,
		W, H, 0, outFormat, GL_UNSIGNED_BYTE, (mImagesData.back().data()));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// work out maximum an-isotropy
	GLfloat max_aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	// set the maximum!
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);

	glGenerateMipmap(GL_TEXTURE_2D);

	Logger::getLogger()->Info("Loaded texture " + filename);
	
	FreeImage_Unload(dib);
}

Texture::Texture(std::string filXP, std::string filXN, std::string filYP, std::string filYN, std::string filZP, std::string filZN) : mType(TextureType::CUBE_MAP_TEXTURE)
{
	mTex = 0;
	glGenTextures(1, &mTex);

	mFilenames = "";

	setActiveTexture();
	
	LoadSideTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, filXN);
	LoadSideTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, filXP); 
	LoadSideTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, filYP);
	LoadSideTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, filYN);
	LoadSideTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, filZN);
	LoadSideTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, filZP);

	glBindTexture(GL_TEXTURE_CUBE_MAP,  mTex);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// work out maximum an-isotropy
	GLfloat max_aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	// set the maximum!
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);

	Logger::getLogger()->Info("Finished loading texture 2d");
}



Texture::Texture(const Texture & t) : mType(t.mType)
{
	mTex = t.mTex;
	W = t.W;
	H = t.H;
}

Texture::~Texture()
{

}

TextureType Texture::getTextureType() const
{
	return mType;
}

GLuint Texture::getTexID() const
{
	return mTex;
}

void Texture::Release()
{
}

int Resources::Texture::setActiveTexture()
{
	switch (mType) {
	case TextureType::DIFFUSE_TEXTURE:
		glActiveTexture(GL_TEXTURE0);
		return 0;
		break;
	case TextureType::SPECULAR_TEXTURE:
		glActiveTexture(GL_TEXTURE1);
		return 1;
		break;
	case TextureType::NORMALS_TEXTURE:
		glActiveTexture(GL_TEXTURE2);
		return 2;
		break;
	case TextureType::CUBE_MAP_TEXTURE:
		glActiveTexture(GL_TEXTURE0);
		return 0;
		break;
	}
	return 0;
}

Resources::Texture::Texture(TextureType type) : mType(type)
{

}

void Texture::LoadFromFile(std::string filename, FIBITMAP * &dib, GLint & inFormat, GLint & outFormat)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	fif = FreeImage_GetFileType(filename.c_str());

	dib = FreeImage_Load(fif, filename.c_str());
	if (!dib) {
		Logger::getLogger()->Error("Can't load " + filename);
		throw MissingResourceException("TEXTURE", filename);
	}

	mFilenames += filename + ";";

	unsigned char* data = FreeImage_GetBits(dib);
	const unsigned int size = FreeImage_GetDIBSize(dib);
	std::vector<unsigned char> ptrData = std::vector<unsigned char>(data, data+size);
	mImagesData.push_back(ptrData);
	W = FreeImage_GetWidth(dib);
	H = FreeImage_GetHeight(dib);


	inFormat = GL_RGBA;
	outFormat = GL_BGRA;

	if (FreeImage_GetBPP(dib) == 24) {
		inFormat = GL_RGB;
		outFormat = GL_BGR;
	}
}


void Texture::LoadFromFile(std::string filename, std::vector<unsigned char> &dest, FIBITMAP * &dib, GLint & inFormat, GLint & outFormat)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	fif = FreeImage_GetFileType(filename.c_str());

	dib = FreeImage_Load(fif, filename.c_str());
	if (!dib) {
		Logger::getLogger()->Error("Can't load " + filename);
		throw MissingResourceException("TEXTURE", filename);
	}

	mFilenames += filename + ";";

	unsigned char* data = FreeImage_GetBits(dib);
	const unsigned int size = FreeImage_GetDIBSize(dib);
	dest = std::vector<unsigned char>(data, data + size);
	W = FreeImage_GetWidth(dib);
	H = FreeImage_GetHeight(dib);


	inFormat = GL_RGBA;
	outFormat = GL_BGRA;

	if (FreeImage_GetBPP(dib) == 24) {
		inFormat = GL_RGB;
		outFormat = GL_BGR;
	}
}

void Texture::LoadSideTexture( GLenum side_target, std::string filename)
{
	FIBITMAP * dib = nullptr;
	GLint inFormat, outFormat;
	LoadFromFile(filename, dib, inFormat, outFormat);

	mTex = 0;

	glBindTexture(GL_TEXTURE_CUBE_MAP, mTex);

	glTexImage2D(
		side_target, 0, inFormat,
		W, H, 0, outFormat, GL_UNSIGNED_BYTE, mImagesData.back().data());

	Logger::getLogger()->Info("Loaded texture " + filename);

	FreeImage_Unload(dib);
}

std::string Resources::Texture::GetFileName() const
{
	return mFilenames;
}
