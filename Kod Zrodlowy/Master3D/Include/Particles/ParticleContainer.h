#pragma once

#include "../Scene/SceneObject.h"
#include "../Resources/ShaderProgram.h"
#include "../Resources/Texture.h"

namespace Particles {
	class ParticleContainer : public SceneObject {
	public:
		ParticleContainer();

		void init(Vec3 position, unsigned int particleCount);
		
		virtual void logic(float deltaTime, const Matrix4x4 &V);
		
		virtual void render(const Matrix4x4 &V, const Matrix4x4 &P);

		unsigned int getParticlesCount() const;

	private:
		struct ParticleData {
			float X = 0, Y = 0, Z = 0;
		};
		
		std::vector<ParticleData> mParticlesData;

		Resources::Texture mTexture;
		Resources::ShaderProgramPtr mShader;

		GLuint mVAO, mVBO;
	};
}