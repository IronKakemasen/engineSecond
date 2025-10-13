#include "PipelineSet.h"
#pragma comment(lib,"d3d12.lib")

PipelineComponents::PipelineComponents()
{
	cur_blendMode = kBlendModeNormal;
	cur_cullMode = CullMode::kBack;
	cur_VPshaderType = ShaderSet::Type::kDefaultAndDefault;
}
