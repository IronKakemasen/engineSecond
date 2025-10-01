#include "directionalLight.h"
#include "../../gpuResources/lightResources/lightResources.h"


void DirectionalLight::CreateResourceAndMapping()
{
	//DirectionalLightResourceの生成
	auto& resource = lightResources->directionalLightResources.emplace_back(
		CreateBufferResource(lightResources->device,sizeof(DirectionalLight)));

	//書き込むためのアドレスを取得
	resource->Map(0, nullptr, reinterpret_cast<void**>(this));

}

DirectionalLight::DirectionalLight()
{
	CreateResourceAndMapping();
}
