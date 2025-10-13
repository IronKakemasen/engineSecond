#include "resourceAndHandleForRender.h"

void ResourceAndHandleForRender::SetDXMatrix(float width_, float height_, float left_, float top_)
{
	//クライアント領域と一緒のサイズにして画面全体に表示
	viewport.Width = static_cast<FLOAT>(width_);
	viewport.Height = static_cast<FLOAT>(height_);
	viewport.TopLeftX = left_;
	viewport.TopLeftY = top_;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	//しざー矩形
	scissorRect.right = static_cast<LONG>(width_);
	scissorRect.bottom = static_cast<LONG>(height_);
	scissorRect.left = static_cast<LONG>(left_);
	scissorRect.top = static_cast<LONG>(top_);
}
