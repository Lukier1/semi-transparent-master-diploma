#include "stdafx.h"
#include <Viewport.h>

Camera::Camera(Vec3 _position, Vec3 _rotation)
{
	position = _position;
	rotation = _rotation;
}

Viewport::Viewport(Vec2 viewSize, const Camera & camera) : mRenderCamera(camera), mViewSize(viewSize)
{
	updatePerspMat();
	updateViewMat();
}

const Matrix4x4 & Viewport::getViewMat() const
{
	return mViewMat;
}

const Matrix4x4 & Viewport::getPerspMat() const
{
	return mPerspMat;
}

void Viewport::setViewSize(const Vec2 & size)
{
	mViewSize = size;
	updatePerspMat();
}

Vec2 Viewport::getViewSize() const
{
	return mViewSize;
}


void Viewport::setPersp(float perNear, float perFar)
{
	const float EPSILON = 0.001f;

	mNear = perNear;
	mFar = perFar;

	//0 mo¿e doprowadziæ do problemów przy obliczaniu macierzy perspektywy
	if (mFar - mNear == 0) {
		mFar += EPSILON;
	}

	updatePerspMat();
}

void Viewport::updateCamera(const Camera & cam)
{
	mRenderCamera = cam;
	updateViewMat();
}

Camera Viewport::getCameraData() const
{
	return mRenderCamera;
}

const float Viewport::getFOV()
{
	return mFOV;
}

void Viewport::setFOV(float FOV)
{
	mFOV = FOV;
	updatePerspMat();
}

/* PRIVATE */
void Viewport::updatePerspMat()
{
	mPerspMat = Matrix4x4::perspective(mNear, mFar, mFOV, mViewSize.v[0] / mViewSize.v[1]);
}

void Viewport::updateViewMat()
{
	mViewMat = Matrix4x4::viewMatrix(mRenderCamera.rotation, mRenderCamera.position);
}


