#pragma once

#include "SceneTransformObject.h"
#include <Resources\ModelLoader.h>
#include <Resources\ShaderProgram.h>
class ModelObject : public SceneTransformObject {
public:
	ModelObject();
	ModelObject(std::shared_ptr<Resources::ModelLoader> model);
	void attachModel(std::shared_ptr<Resources::ModelLoader> model);
	void draw(const Resources::ShaderProgram * shader, const Matrix4x4 &perspectiveMatrix, const Matrix4x4 &viewMatrix);
	void drawTransparent(const Resources::ShaderProgram* shader, const Matrix4x4& perspectiveMatrix, const Matrix4x4& viewMatrix);
private:
	std::shared_ptr<Resources::ModelLoader> mModel;
};