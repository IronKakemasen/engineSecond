#pragma once
#include "../lightBasicStructure.h"



struct DirectionalLight :public LightBasicStructure
{
	virtual void CreateResourceAndMapping()override;

	DirectionalLight();

private:
	LightResources* lightResources = nullptr;

};

