#pragma once
#include <d3d12.h>

struct CommandControll;

namespace BarrierControll
{
	D3D12_RESOURCE_BARRIER Create(
		ID3D12Resource* dst_resource_,
		D3D12_RESOURCE_BARRIER_TYPE Type,
		D3D12_RESOURCE_BARRIER_FLAGS Flags,
		D3D12_RESOURCE_STATES before_,
		D3D12_RESOURCE_STATES after_);

	void Pitch(CommandControll* commanndControll_, D3D12_RESOURCE_BARRIER barrier_);

}

