#include "barrierControll.h"
#include "../commandControll/commandControll.h"

#pragma comment(lib,"d3d12.lib")


D3D12_RESOURCE_BARRIER BarrierControll::Create(
	ID3D12Resource* dst_resource_,
	D3D12_RESOURCE_BARRIER_TYPE Type,
	D3D12_RESOURCE_BARRIER_FLAGS Flags,
	D3D12_RESOURCE_STATES before_,
	D3D12_RESOURCE_STATES after_)
{
	D3D12_RESOURCE_BARRIER ret_barrier{};
	
	//バリアの設定
	ret_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	ret_barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファーに対して行う
	ret_barrier.Transition.pResource = dst_resource_;

	//バリアステートを遷移
	ret_barrier.Transition.StateBefore = before_;
	ret_barrier.Transition.StateAfter = after_;

	return ret_barrier;
}

void BarrierControll::Pitch(CommandControll* commanndControll_, D3D12_RESOURCE_BARRIER barrier_)
{
	//TransitionBarrierを張る
	commanndControll_->commandList->ResourceBarrier(1, &barrier_);

}

