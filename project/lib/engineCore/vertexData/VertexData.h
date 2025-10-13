#pragma once
#include "../../utilities/vectorAndMatrix/vectorAndMatrix.h"

struct VertexData
{
	Vector3 position;
	Vector2 texcoord;
	Vector3 normal;

	void Set(Vector3 vec_, Vector3 diffVec1_, Vector3 diffVec2_, Vector2 texcoord_);

};
