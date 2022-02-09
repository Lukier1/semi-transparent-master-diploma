#pragma once

#include "../stdafx.h"
#include "CobMath.h"

struct Camera {
	Vec3 position = Vector3f(0, 2.0f, 5.0f);
	Vec3 rotation = Vector3f(0.4f, -3.14f, 0);

	Camera(Vec3 position, Vec3 rotation);
};

class Viewport {
public:
	Viewport(Vec2 viewSize, const Camera &camera);

	const Matrix4x4 & getViewMat() const;
	const Matrix4x4 & getPerspMat() const;
	
	void setViewSize(const Vec2& size);
	Vec2 getViewSize() const;

	void setPersp(float perNear, float perFar);

	void updateCamera(const Camera & cam);
	Camera getCameraData() const;

	const float getFOV();
	void setFOV(float FOV);
private:
	Matrix4x4 mViewMat;
	Matrix4x4 mPerspMat;
	Vec2 mViewSize;
	float mNear = 0.1f, mFar = 100.0f, mFOV = 3.0f/PI;

	Camera mRenderCamera;

	void updatePerspMat();
	void updateViewMat();
};