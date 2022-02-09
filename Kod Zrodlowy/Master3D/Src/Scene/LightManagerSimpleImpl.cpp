#include "stdafx.h"

#include <Scene\LightManagerSimpleImpl.h>

LightManagerSimpleImpl::LightManagerSimpleImpl() 
{

}

LightManagerSimpleImpl::~LightManagerSimpleImpl()
{
	mPointLightsList.clear();
	mDirectionLightsList.clear();
}

const std::vector<PointLight>& LightManagerSimpleImpl::getPointLightList() const
{
	return mPointLightsList;
}

std::vector<DirectionLight>& LightManagerSimpleImpl::getDirectionLightList() 
{
	return mDirectionLightsList;
}

void LightManagerSimpleImpl::addLight(const DirectionLight & light)
{
	mDirectionLightsList.push_back(light);
}

void LightManagerSimpleImpl::addLight(const PointLight & light)
{
	mPointLightsList.push_back(light);
}
