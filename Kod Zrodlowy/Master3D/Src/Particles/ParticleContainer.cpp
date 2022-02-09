#include "stdafx.h"

#include <Particles/ParticleContainer.h>


using namespace Particles;
using namespace Resources;

Particles::ParticleContainer::ParticleContainer() : mTexture("Data/Images/particle.png", DIFFUSE_TEXTURE) , mVBO(0), mVAO(0)
{
	ShaderBuilder builder;
	mShader = builder
		.AddShader("Shaders/bill_vert.glsl", VERTEX)
		.AddShader("Shaders/bill_frag.glsl", FRAGMENT)
		.AddShader("Shaders/bill_geom.glsl", GEOMETRY)
		.Build();
}

void ParticleContainer::init(Vec3 position, unsigned int particleCount) {
	setPosition(position);
	mParticlesData = std::vector<ParticleData>(particleCount);

	srand(particleCount);
	for (unsigned int i = 0; i < particleCount; ++i)
	{
		mParticlesData[i].X = static_cast<float>(((rand() % 100) - 50) / 10.0f);
		mParticlesData[i].Y = static_cast<float>(((rand() % 100) - 50) / 10.0f);
		mParticlesData[i].Z = static_cast<float>(((rand() % 100) - 50) / 10.0f);
	}

	mVBO = 0;
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleData)*particleCount, &mParticlesData[0], GL_STREAM_DRAW);

	mVAO = 0;
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void ParticleContainer::logic(float deltaTime, const Matrix4x4 &V) {
	//TODO Move particles and sorting
	struct SortData {
		float camera_dis;
		unsigned int index;
	};
	std::vector<SortData> toSortTable(mParticlesData.size());
	const float * matrix = V.getMatrixData();

	for (unsigned int i = 0; i < mParticlesData.size(); ++i)
	{
		toSortTable[i].index = i;
		float camera_dis = matrix[11];
		camera_dis += mParticlesData[i].X * matrix[8];
		camera_dis += mParticlesData[i].Y * matrix[9];
		camera_dis += mParticlesData[i].Z * matrix[10];

		toSortTable[i].camera_dis = camera_dis;
	}
	/*
	std::sort(toSortTable.begin(), toSortTable.end(),
		[](const SortData & a, const SortData & b) -> bool
	{
		return a.camera_dis < b.camera_dis;
	});*/

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	float * dataMap = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

	
	if (dataMap) {
		for (unsigned int i = 0; i < mParticlesData.size(); ++i)
		{
			dataMap[i * 3] = mParticlesData[toSortTable[i].index].X;
			dataMap[i * 3 + 1] = mParticlesData[toSortTable[i].index].Y;
			dataMap[i * 3 + 2] = mParticlesData[toSortTable[i].index].Z;
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	return;
}

void ParticleContainer::render(const Matrix4x4 &V, const Matrix4x4 &P) {
	mShader->UseProgram();

	mShader->applyTexture(mTexture.getTextureType(), mTexture.getTexID());
	Matrix4x4 mvp;
	Matrix4x4 M = Matrix4x4::translate(getPosition().v[0], getPosition().v[1], getPosition().v[2]);
	mvp = P*V*M;
	mShader->apply("mvp_mat", mvp);
	mShader->apply("P_mat", P);


	glBindVertexArray(mVAO);
	glDrawArrays(GL_POINTS, 0, getParticlesCount());
}

unsigned int ParticleContainer::getParticlesCount() const {
	return mParticlesData.size();
}


