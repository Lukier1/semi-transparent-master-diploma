#pragma once

#include "LightManager.h"

class LightManagerSimpleImpl : public LightManager
{
public:
	LightManagerSimpleImpl();
	~LightManagerSimpleImpl();

	// Inherited via LightManager
	virtual const std::vector<PointLight>& getPointLightList() const override;

	virtual void addLight(const PointLight & light) override;

	virtual std::vector<DirectionLight>& getDirectionLightList()  override;
	virtual void addLight(const DirectionLight &light) override;
private:
	std::vector<PointLight> mPointLightsList;
	std::vector<DirectionLight> mDirectionLightsList;
};

