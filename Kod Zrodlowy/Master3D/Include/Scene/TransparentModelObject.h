#pragma once

#include "ModelObject.h"
#include <Math\Vec.h>

class TransparentModelObject : public ModelObject {
public:
	TransparentModelObject() : ModelObject() {

	}

	TransparentModelObject(std::shared_ptr<Resources::ModelLoader> model) : ModelObject(model) {

	}

	float getAlpha() const {
		return alpha;
	}

	float getEta() const {
		return eta;
	}

	Vec3 getTransmitive() const {
		return transmitive;
	}

	Vec3 getColor() const {
		return transmitive;
	}

	bool getIsTextured() const {
		return isTextured;
	}

	TransparentModelObject setAlpha(float _alpha) {
		this->alpha = _alpha;
		return *this;
	}

	TransparentModelObject setEta(float _eta) {
		this->eta = _eta;
		return *this;
	}

	TransparentModelObject setTransmitive(Vec3 _transmitive) {
		this->transmitive = _transmitive;
		return *this;
	}

	TransparentModelObject setColor(Vec3 _color) {
		this->color = _color;
		return *this;
	}
	TransparentModelObject setIsTextured(bool value) {
		this->isTextured = value;
		return *this;
	}
private:
	float alpha;
	float eta;
	Vec3 transmitive;
	Vec3 color;
	bool isTextured = false;
};