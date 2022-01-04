#pragma once

#include "Resource.h"
#include "../CobMath.h"
#include "TextureType.h"

namespace Resources {
	class ShaderBuilder;
	
	struct ShaderFile {
		GLuint shaderId = 0;
		std::string filename;
		bool loaded = false;
	};

	const short SHADER_TYPE_NUM = 3;

	enum SHADER_TYPE {
		VERTEX = 0,
		FRAGMENT = 1,
		GEOMETRY = 2
	};

	class ShaderProgram : public Resource
	{
	public:		
		bool IsProgramGenerated() const;

		void UseProgram() const;
		GLuint getProgramId() const;

		virtual void Release() override;
		virtual std::string GetFileName() const override;
		
		std::string GetShadersFileName(SHADER_TYPE type) const;


		~ShaderProgram();

		void apply(const std::string &locator, const Matrix4x4 &mat) const;
		void apply(const std::string &locator, const Vec2 &vec) const;
		void apply(const std::string &locator, const Vec3 &vec) const;
		void apply(const std::string &locator, const Vec3 * vec,unsigned int count) const;
		void apply(const std::string &locator, const Vec4 &vec) const;
		void apply(const std::string &locator, float val) const;
		void apply(const std::string &locator, float * val, unsigned int count) const;
		void apply(const std::string &locator, GLuint val) const;
		void apply(const std::string &locator, int val) const;
		void applyTexture(GLuint texNum, GLuint texId, bool cubeTex) const;
		void applyTexture(TextureType texType, GLuint texId) const;

		ShaderProgram(GLuint programId, std::string shaderFilename[], unsigned short type_count);

	
	private:
	
		const unsigned short TYPE_COUNT;

		bool isProgramGenerated;
		GLuint programId;
		GLuint * shaderId;
		std::string shaderFilenames[SHADER_TYPE_NUM];
	};
	
	class ShaderBuilder {
	public:
		ShaderBuilder();

		//@throws IOException
		ShaderBuilder& AddShader(std::string filename, SHADER_TYPE type);

		std::shared_ptr<ShaderProgram> Build();
	private:
		static const unsigned short TYPE_COUNT = SHADER_TYPE_NUM;

		ShaderFile mFiles[TYPE_COUNT];

		GLuint ResolveShaderId(SHADER_TYPE type) const;	

		const unsigned int NO_SHADER_LOADED ;

	};

	typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;
}