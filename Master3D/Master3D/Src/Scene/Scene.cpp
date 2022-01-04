#include "stdafx.h"
#include <Scene/Scene.h>
#include <Scene/PointLight.h>
#include <Scene/DirectionLight.h>
#include <Scene/LightManagerSimpleImpl.h>

void SceneManager::applyLights(const ShaderProgram* program) {
	const auto pointLightList = mLightManager->getPointLightList();
	const auto lightDirList = mLightManager->getDirectionLightList();

	program->apply("lightNumber", pointLightList.size());
	if (pointLightList.size() != 0) {
		std::vector<Vec3> positions;
		std::vector<Vec3> colors;
		std::vector<float> powers;

		for (PointLight light : pointLightList) {
			positions.push_back(light.getPosition());
			colors.push_back(light.GetDiffusionColor());
			powers.push_back(light.GetPower());
		}

		program->apply("lightsPositions", &positions[0], positions.size());
		program->apply("lightsColors", &colors[0], colors.size());
		program->apply("lightsPowers", &powers[0], powers.size());
	}

	program->apply("dirLightNumber", lightDirList.size());
	if (lightDirList.size() != 0) {
		std::vector<Vec3> dirs;
		std::vector<Vec3> colors;

		for (DirectionLight light : lightDirList) {
			dirs.push_back(light.GetDirection());
			colors.push_back(light.GetDiffusionColor());
		}
		if (lightDirList.size() != 0) {
			program->apply("dirLightDirection", &dirs[0], dirs.size());
			program->apply("dirLightColor", &colors[0], colors.size());
		}
	}

	program->apply("ambientLight", ambientLight);
}

void SceneManager::drawTransparentObjects(const ShaderProgram* alphaPassProg, const Matrix4x4& perspMat, const Matrix4x4& viewMat) {

	for (TransparentModelObject transparentModelObject : mTransparentObjects) {
		applyTransparentObjectToShader(transparentModelObject, alphaPassProg);
		alphaPassProg->apply("color", transparentModelObject.getColor());
		alphaPassProg->apply("isTextured", (int)(transparentModelObject.getIsTextured()));
		transparentModelObject.draw(alphaPassProg, perspMat, viewMat);
	}

	//mAlphaParticle->RenderMulti(perspMat, viewMat, alphaPassProg);
	/*int index = 0;
	for (ModelObject object : mIntransparentObjects) {
		alphaPassProg->apply("isTextured", (int)(true));
		alphaPassProg->apply("ind", index++);
		object.drawTransparent(alphaPassProg, perspMat, viewMat);
	}*/
}

void SceneManager::drawIntransparentObjects(const ShaderProgram* shader, const Matrix4x4& perspMat, const Matrix4x4& viewMat) {
	int index = 0;
	for (ModelObject object : mIntransparentObjects) {
		shader->apply("ind", index++);
		object.draw(shader, perspMat, viewMat);
	}
}

void SceneManager::applyTransparentObjectToShader(const TransparentModelObject& object, const ShaderProgram* shader) {
	shader->apply("alpha", object.getAlpha());
	shader->apply("eta", object.getEta());
	shader->apply("transmitive", object.getTransmitive());
}

void TestScene::kulkaScene() {

	/*mTestObject.attachModel(mModel);
	mTestObject.setScale(Vector3f(25, 25, 25));
	mTestObject.setRotation(Vector3f(0, 90, 0));
	mTestObject.setPosition(Vector3f(0, -0.05, 0.2));
	mIntransparentObjects.push_back(mTestObject);

	mTestAlphaBig.attachModel(mSphereAlphaModel);
	mTestAlphaBig.setScale(Vector3f(4, 4, 4));
	mTestAlphaBig.setPosition(Vector3f(0, 2, 2));
	mTestAlphaBig.setAlpha(0.8f);
	mTestAlphaBig.setEta(0.75f);
	mTestAlphaBig.setTransmitive(Vector3f(0.7f, 0.0f, 0.0f));


	mTestAlphaSmall.attachModel(mSphereAlphaModel);
	mTestAlphaSmall.setScale(Vector3f(2, 2, 2));
	mTestAlphaSmall.setPosition(Vector3f(1, 2, 0));
	mTestAlphaSmall.setAlpha(0.8f);
	mTestAlphaSmall.setEta(0.41f);
	mTestAlphaSmall.setTransmitive(Vector3f(0.0f, 1.0f, 0.0f));

	mCrateObject.attachModel(mCrate);
	mCrateObject.setScale(Vector3f(0.5, 0.5, 0.5));
	mCrateObject.setPosition(Vector3f(7, 4, 0));
	mCrateObject.setRotation(Vector3f(0, 90, 0));
	mIntransparentObjects.push_back(mCrateObject);

	mCrateObject.attachModel(mCrate);
	mCrateObject.setScale(Vector3f(0.5, 0.5, 0.5));
	mCrateObject.setPosition(Vector3f(2, 4, 0));
	mCrateObject.setRotation(Vector3f(0, 90, 0));
	mIntransparentObjects.push_back(mCrateObject);*/

	srand(1000);
	for (int z = 0; z < 3; ++z) {
		for (int x = 0; x < 3; ++x) {
			for (int y = 0; y < 3; ++y) {
				mTransparentObjects.push_back(TransparentModelObject());
				mTransparentObjects.back().attachModel(mSphereAlphaModel);
				mTransparentObjects.back().setScale(Vector3f(1.4, 1.4, 1.4));
				mTransparentObjects.back().setPosition(Vector3f(x * 3 - 3, 4 + z * 3, -y * 3 + 3));
				//int x = rand();
				mTransparentObjects.back().setAlpha(0.1f);
				mTransparentObjects.back().setEta(1.0f);
				//mTransparentObjects.back().setEta(0.7f);
				mTransparentObjects.back().setIsTextured(false);

				Vec3 color = Vector3f((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f);
				if (z == 2 && x == 2) {
					std::cout << (int)(255 * color.v[0]) << (int)(255 * color.v[1]) << (int)(255 * color.v[2]) << std::endl;
				}
				mTransparentObjects.back().setTransmitive(color);
				mTransparentObjects.back().setColor(color);
			}
		}
	}
}

 
void TestScene::loadScene() {
	mLightManager = std::unique_ptr<LightManager>(new LightManagerSimpleImpl());
	mLightManager->addLight(DirectionLight()
		.SetDirection(Vector3f(0.0f, -1.0f, -1.0f).normalizeVec3())
		.SetDiffusionColor(Vector3f(1.0, 1.0f, 1.0f))
		.SetReflectionColor(Vector3f(1.0f, 1.0f, 1.0f)));
	//mModel = std::auto_ptr<ModelLoader>(new  ModelLoader("Data/Meshes/metric_test_2.obj", false));
	mSphereAlphaModel = std::auto_ptr<ModelLoader>(new ModelLoader("Data/Meshes/kula.obj", true, false));
	
	//mCrate = std::auto_ptr<ModelLoader>(new ModelLoader("Data/Meshes/box.obj", false));
	//mModel = std::unique_ptr<ModelLoader>(new  ModelLoader("Data/Meshes/sponza2/untitled.obj", false));
	//mAlphaParticle = new Particles::AlphaParticle(Texture("Data/Images/smoke.png", TextureType::DIFFUSE_TEXTURE));
	//mAlphaParticle->Init();

	kulkaScene();
}

void TestScene::setAlpha(float alpha)
{
	for (int i = 0; i < mTransparentObjects.size(); ++i) {
		mTransparentObjects[i].setAlpha(alpha);
	}
}

void TestScene::randAlpha(float seed) {
	srand((unsigned int)(seed * 124.0f));
	for (int i = mTransparentObjects.size()-1; i >= 0; --i) {
		mTransparentObjects[i].setAlpha((rand()%100)/200.0f);
	}
}
