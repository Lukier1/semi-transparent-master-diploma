#pragma once

#include "CobMath.h"
#include "TransparentModelObject.h"
#include "ModelObject.h"
#include "../Resources/ShaderProgram.h"
#include "../Resources/ModelLoader.h"
#include <Scene/LightManager.h>
#include <Particles/AlphaParticle.h>

using namespace Resources;

class SceneManager {
protected:
	std::vector<TransparentModelObject> mTransparentObjects;
	std::vector<ModelObject> mIntransparentObjects;
	std::unique_ptr<LightManager> mLightManager;

	const Vec3 ambientLight = Vector3f(0.3f, 0.3f, 0.3f);
	Particles::AlphaParticle* mAlphaParticle;
public:
	void drawTransparentObjects(const ShaderProgram* alphaPassProg, const Matrix4x4& perspMat, const Matrix4x4& viewMat);

	void drawIntransparentObjects(const ShaderProgram* shader, const Matrix4x4& perspMat, const Matrix4x4& viewMat);
	void applyLights(const ShaderProgram* program);
private:
	void applyTransparentObjectToShader(const TransparentModelObject& object, const ShaderProgram* shader);
};

class TestScene : public SceneManager {
private:
	std::shared_ptr<ModelLoader> mModel, mSphereAlphaModel, mGlassModel, mCrate;

	ModelObject mTestObject, mCrateObject;
	TransparentModelObject mTestAlphaBig, mTestAlphaSmall, mGlassObject;
	void kulkaScene();
public:
	void loadScene();
	void setAlpha(float alpha);
	void randAlpha(float seed);
};