#include "stdafx.h"
#include <Resources/ShaderProgram.h>
#include <Exceptions\IOException.h>

#include <limits>
using namespace Resources;


bool ShaderProgram::IsProgramGenerated() const {
	return isProgramGenerated;
}

GLuint ShaderProgram::getProgramId() const {
	return programId;
}

void ShaderProgram::Release() {

	if (programId != -1)
	{
		glDeleteProgram(programId);
		isProgramGenerated = false;
		programId = (GLuint)-1;
	}
	if (shaderId != nullptr)
	{
		for (int i = 0; i < TYPE_COUNT; ++i) {
			if (shaderId[i] != (GLuint)-1) {
				glDeleteProgram(shaderId[i]);
				shaderId[i] = (GLuint)-1;
			}
		}

		delete[] shaderId;
		shaderId = nullptr;
	}
	
}

void ShaderProgram::apply(const std::string &locator, const Matrix4x4 &mat) const {
	GLuint matrix_location = glGetUniformLocation(programId, locator.c_str());
	glUniformMatrix4fv(matrix_location, 1, GL_TRUE, mat.getMatrixData());
}
void Resources::ShaderProgram::apply(const std::string &locator, const Vec2 & vec) const
{
	GLuint location = glGetUniformLocation(programId, locator.c_str());
	glUniform2fv(location, 1, vec.getData());
}
void ShaderProgram::apply(const std::string &locator, const Vec3 &vec) const {
	GLuint location = glGetUniformLocation(programId, locator.c_str());
	glUniform3fv(location, 1, vec.getData());
}

void ShaderProgram::apply(const std::string &locator, const Vec3 * vec, unsigned int count) const {
	GLuint location = glGetUniformLocation(programId, locator.c_str());
	glUniform3fv(location, count, (const float*)(vec));
}

void Resources::ShaderProgram::apply(const std::string &locator, const Vec4 & vec) const
{
	GLuint location = glGetUniformLocation(programId, locator.c_str());
	glUniform4fv(location, 1, vec.getData());
}

void ShaderProgram::apply(const std::string& locator, float val) const {
	GLuint location = glGetUniformLocation(programId, locator.c_str());
	glUniform1f(location, val);

}
void ShaderProgram::apply(const std::string& locator, float * val, unsigned int count) const {
	GLuint location = glGetUniformLocation(programId, locator.c_str());
	glUniform1fv(location, count, val);

}
void ShaderProgram::apply(const std::string& locator, GLuint val) const
{
	GLuint location = glGetUniformLocation(programId, locator.c_str());
	glUniform1ui(location, val);
}
void ShaderProgram::apply(const std::string& locator, int val) const
{
	GLuint location = glGetUniformLocation(programId, locator.c_str());
	glUniform1i(location, val);
}

void Resources::ShaderProgram::applyTexture(GLuint texNum, GLuint texId, bool cubeTex) const
{

	glActiveTexture(texNum);
	if (cubeTex) {
		glBindTexture(GL_TEXTURE_2D, texId);
	}
	else
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, texId);
	}
}
void ShaderProgram::applyTexture(TextureType texType, GLuint texId) const
{
	std::string locator;
	switch (texType)
	{
	case DIFFUSE_TEXTURE:
		apply("diffuse_texture", 0);
		glActiveTexture(GL_TEXTURE0);
		break;
	case SPECULAR_TEXTURE:
		apply("specular_texture", 1);
		glActiveTexture(GL_TEXTURE1);
		break;
	case NORMALS_TEXTURE:
		apply("normals_texture", 2);
		glActiveTexture(GL_TEXTURE2);
		break;
	case CUBE_MAP_TEXTURE:
		apply("cube_texture", 0);
		glActiveTexture(GL_TEXTURE0);
		break;
	default:
		glActiveTexture(GL_TEXTURE0);
		break;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (texType != CUBE_MAP_TEXTURE) {
		glBindTexture(GL_TEXTURE_2D, texId);
	}
	else
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, texId);
	}
}

Resources::ShaderProgram::ShaderProgram(GLuint programId,  std::string shaderFilename[], unsigned short type_count) : TYPE_COUNT(type_count)
{
	this->programId = programId;
	this->shaderId = new GLuint[TYPE_COUNT];

	for (int i = 0; i < TYPE_COUNT; ++i) {
		this->shaderFilenames[i] = shaderFilename[i];
	}
	isProgramGenerated = true;
}



void ShaderProgram::UseProgram() const
{
	glUseProgram(programId);
}
ShaderProgram::~ShaderProgram() {
	Release();
}


std::string ShaderProgram::GetFileName() const
{
	std::string compose;
	for (int i = 0; i < TYPE_COUNT; ++i)
	{
		compose += shaderFilenames[i] + ";";
	}
	return compose;
}

std::string Resources::ShaderProgram::GetShadersFileName(SHADER_TYPE type) const
{
	return shaderFilenames[type];
}




Resources::ShaderBuilder::ShaderBuilder() : NO_SHADER_LOADED(UINT_MAX)
{

}

ShaderBuilder& ShaderBuilder::AddShader(std::string filename, SHADER_TYPE type)
{
	mFiles[(int)type].filename = filename;
	mFiles[(int)type].loaded = false;

	return *this;

}

GLuint ShaderBuilder::ResolveShaderId(SHADER_TYPE type) const {
	switch (type)
	{
	case VERTEX:
		return GL_VERTEX_SHADER;
		break;
	case FRAGMENT:
		return GL_FRAGMENT_SHADER;
		break;
	case GEOMETRY:
		return GL_GEOMETRY_SHADER;
		break;
	default:
		return (GLuint)-1;
		break;
	}
}

std::shared_ptr<ShaderProgram> Resources::ShaderBuilder::Build()
{
	//Compile program
	for (int i = 0; i < TYPE_COUNT; ++i) {
		if (!mFiles[i].filename.empty()) {

			std::string fileText;
			std::string filename = mFiles[i].filename;
			std::fstream file;

			file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			try {
				file.open(filename, std::fstream::in); // catch error
				fileText = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				file.close();
			}
			catch (std::fstream::failure e)
			{
				Logger::getLogger()->Error("Can't open file: " + filename);
				throw IOException("Can't open shader file: " + filename);
			}

			const char* p_text = fileText.c_str();
			GLuint shaderout = glCreateShader(ResolveShaderId((SHADER_TYPE)i));
			glShaderSource(shaderout, 1, &p_text, NULL);
			glCompileShader(shaderout);

			GLint isCompiled = 0;
			glGetShaderiv(shaderout, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shaderout, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> errorLog(maxLength);
				std::unique_ptr<char> out(new char[maxLength + 1]);
				out.get()[maxLength] = 0;
				glGetShaderInfoLog(shaderout, maxLength, &maxLength, out.get());

				std::string description(out.get());

				Logger::getLogger()->Error("Compilation error file: " + filename + "\nerrors:\n" + description);

				glDeleteShader(shaderout); // Don't leak the shader.

				mFiles[i].loaded = false;
				//TODO Change exception
				throw new std::exception("Cant compile shader");
			}
			else
			{
				mFiles[i].loaded = true;
				mFiles[i].shaderId = shaderout;
			}
		}
	}
	//Link program
	GLuint shader_programme = glCreateProgram();

	std::string filenames[TYPE_COUNT];
	for (int i = 0; i < TYPE_COUNT; ++i) {
		if (mFiles[i].loaded) {
			glAttachShader(shader_programme, mFiles[i].shaderId);
			filenames[i] = mFiles[i].filename;
		}
		else
		{
			filenames[i] = "";
		}
	}
	glLinkProgram(shader_programme);
	Logger::getLogger()->Info("Program compilation succes");

	
	auto shader = std::shared_ptr<ShaderProgram>(new ShaderProgram(shader_programme, filenames, TYPE_COUNT));
	return shader;
}
