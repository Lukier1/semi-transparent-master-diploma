#include "stdafx.h"

#include <Scene\ModelObject.h>
#include <Resources\Mesh.h>

using namespace Resources;

const std::string MVP_MAT = "MVPMat";
const std::string MV_MAT = "MVMat";
const std::string M_MAT = "MMat";
const std::string P_MAT = "PMat";
const std::string V_MAT = "VMat";

ModelObject::ModelObject() {

}

ModelObject::ModelObject(std::shared_ptr<Resources::ModelLoader> model) {
	mModel = model;
}

void ModelObject::attachModel(std::shared_ptr<Resources::ModelLoader> model)
{
	mModel = model;
}

void ModelObject::draw(const ShaderProgram * shader, const Matrix4x4 &perspectiveMatrix, const Matrix4x4 &viewMatrix)
{
	const Matrix4x4 worldMatrix = getTransmformationMatrix();
	const Matrix4x4 mvMatrix = worldMatrix*viewMatrix;
	const Matrix4x4 mvpMatrix = mvMatrix*perspectiveMatrix;

	shader->apply(MVP_MAT, mvpMatrix);
	shader->apply(MV_MAT, mvMatrix);
	shader->apply(M_MAT, worldMatrix);
	shader->apply(P_MAT, perspectiveMatrix);
	shader->apply(V_MAT, viewMatrix);

	auto meshList = mModel->getMeshList();
	for (Mesh * mesh : meshList)
	{
		if (mesh->getMaterialIndex() != 0 && mModel->isMaterialsEnabled())
		{
			float opacity = mModel->getMaterialList()[mesh->getMaterialIndex()]->GetFloat(Material::PARAM_TYPE::OPACITY);
			if (opacity < 1.0 && opacity > 0.0) {
				continue;
			}
			mModel->getMaterialList()[mesh->getMaterialIndex()]->ApplyParams(shader);
		}
		mesh->Draw();
	}
}

void ModelObject::drawTransparent(const ShaderProgram* shader, const Matrix4x4& perspectiveMatrix, const Matrix4x4& viewMatrix)
{
	const Matrix4x4 worldMatrix = getTransmformationMatrix();
	const Matrix4x4 mvMatrix = worldMatrix * viewMatrix;
	const Matrix4x4 mvpMatrix = mvMatrix * perspectiveMatrix;

	shader->apply(MVP_MAT, mvpMatrix);
	shader->apply(MV_MAT, mvMatrix);
	shader->apply(M_MAT, worldMatrix);
	shader->apply(P_MAT, perspectiveMatrix);
	shader->apply(V_MAT, viewMatrix);

	auto meshList = mModel->getMeshList();
	for (Mesh* mesh : meshList)
	{
		if (mesh->getMaterialIndex() != 0 && mModel->isMaterialsEnabled())
		{
			float opacity = mModel->getMaterialList()[mesh->getMaterialIndex()]->GetFloat(Material::PARAM_TYPE::OPACITY);
			if (opacity < 1.0 && opacity > 0.0) {
				shader->apply("alpha", opacity);
				shader->apply("eta", 1.0f);
				shader->apply("transmitive", Vector3f(1,0,0));
				shader->apply("color", Vector3f(1, 0, 0));
				mModel->getMaterialList()[mesh->getMaterialIndex()]->ApplyParams(shader);
				mesh->Draw();
			}
			
		//	mModel->getMaterialList()[mesh->getMaterialIndex()]->
			
		}
		
	}
}