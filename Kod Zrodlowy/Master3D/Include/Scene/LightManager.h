#pragma once

#include "PointLight.h"
#include "DirectionLight.h"

class LightManager {
public:
	//TODO Add point light list
	virtual std::vector<DirectionLight>& getDirectionLightList() = 0;
	virtual const std::vector<PointLight>& getPointLightList() const = 0;
	virtual void addLight(const PointLight &light) = 0;
	virtual void addLight(const DirectionLight &light) = 0;

};