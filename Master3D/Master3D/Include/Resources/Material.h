#pragma once

#include <map>
#include "../CobMath.h"
#include "Texture.h"
#include "ShaderProgram.h"
namespace Resources {
	class Material {
	public:
		enum PARAM_TYPE {
			DIFFUSE_COLOR,
			AMBIENT_COLOR,
			SPECULAR_COLOR,
			SPEC_POWER,
			OPACITY
		};

		void SetParam(PARAM_TYPE type, const Vec3 &vec);
		void SetParam(PARAM_TYPE type, const float val);
		
		float GetFloat(PARAM_TYPE type);

		void SetTexture(const Texture * tex);

		void ApplyParams(const ShaderProgram * program);
	private:
		static const GLuint NOMRALS_MAP_MODE = 0x1;
		static const GLuint SPECULAR_MAP_MODE = 0x2;


		std::map<PARAM_TYPE, Vec3> mVec3Map;
		std::map<PARAM_TYPE, float> mFloatMap;
		std::map<TextureType, GLuint> mTextureMap;
	};
}