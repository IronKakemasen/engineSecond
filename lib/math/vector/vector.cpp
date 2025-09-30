#include "vector.h"
#include "../matrix/matrix.h"
#include <math.h>

//=======================================[ Vector2 ]=======================================
float Vector2::GetMagnitutde()
{
	float ret_mag = sqrtf(x * x + y * y);
	return ret_mag;
}

Vector2 Vector2::GetNormalized()
{
	if (x == 0.0f && y == 0.0f)
	{
		return { 0,0 };
	}

	float inv_mag = 1.0f / GetMagnitutde();

	return { x * inv_mag ,y * inv_mag };
}

float Vector2::GetCos(Vector2 other_)
{
	//正規化する
	Vector2 normalized1 = GetNormalized();
	Vector2 normalized2 = other_.GetNormalized();

	//cos(🍆角)
	float ret_cos = normalized1.x * normalized2.x + normalized1.y * normalized2.y;

	return ret_cos;

}

float Vector2::GetDot(Vector2 other_)
{
	float ret_dot = x * other_.x + y * other_.y;
	return ret_dot;
}


Vector2 operator+(Vector2 const& lVec_, Vector2 const& rVec_)
{
	return { lVec_.x + rVec_.x,lVec_.y + rVec_.y };
}


Vector2 operator-(Vector2  const& lVec_, Vector2  const& rVec_)
{
	return { lVec_.x - rVec_.x,lVec_.y - rVec_.y };

}
Vector2 operator*(Vector2 const& dst_vec_, float const multipleNum)
{
	return { dst_vec_.x * multipleNum,dst_vec_.y * multipleNum };
}






//=======================================[ Vector3 ]=======================================
Vector3 Vector3::GetMultiply(Matrix4 src_mat_)
{
	Vector3 ret_multiplied;

	ret_multiplied.x = x * src_mat_.m[0][0] + y * src_mat_.m[1][0] + z * src_mat_.m[2][0] + src_mat_.m[3][0];
	ret_multiplied.y = x * src_mat_.m[0][1] + y * src_mat_.m[1][1] + z * src_mat_.m[2][1] + src_mat_.m[3][1];
	ret_multiplied.z = x * src_mat_.m[0][2] + y * src_mat_.m[1][2] + z * src_mat_.m[2][2] + src_mat_.m[3][2];


	return ret_multiplied;
}

float Vector3::GetMagnitutde()
{
	return sqrtf(x * x + y * y + z * z);
}

//Cosを計算して返す
float Vector3::GetCos(Vector3 other_)
{
	//正規化する
	Vector3 normalized1 = GetNormalized();
	Vector3 normalized2 = other_.GetNormalized();

	//cos(🍆角)
	float ret_cosTheta =
		normalized1.x * normalized2.x +
		normalized1.y * normalized2.y +
		normalized1.z * normalized2.z;

	return ret_cosTheta;
}

// 正規化ベクトルを返す
Vector3 Vector3::GetNormalized()
{
	if (x == 0.0f && y == 0.0f && z == 0.0f)
	{
		return { 0.0f,0.0f,0.0f };
	}

	float inv_mag = 1.0f / GetMagnitutde();

	Vector3 ret_normalizeVec = { x * inv_mag, y * inv_mag, z * inv_mag };

	return ret_normalizeVec;
}

//外積
Vector3 Vector3::GetCross(Vector3 other)
{
	Vector3 retCross;

	retCross.x = y * other.z - z * other.y;
	retCross.y = z * other.x - x * other.z;
	retCross.z = x * other.y - y * other.x;

	return retCross;
}

float Vector3::GetDot(Vector3 other_)
{
	float ret_dot = x * other_.x + y * other_.y + z * other_.z;

	return ret_dot;
}

Vector3 operator+(Vector3 const& lVec, Vector3 const& rVec2)
{
	Vector3 ret_vec = { lVec.x + rVec2.x,lVec.y + rVec2.y ,lVec.z + rVec2.z };

	return ret_vec;
}

bool operator==(Vector3 const& lVec, Vector3  const& rVec2)
{
	bool ret = false;
	if (lVec.x == rVec2.x && lVec.y == rVec2.y && lVec.z == rVec2.z) ret = true;

	return ret;
}

Vector3 operator*(Vector3 const& dst_vec, float const multipleNum)
{
	return { dst_vec.x * multipleNum,dst_vec.y * multipleNum ,dst_vec.z * multipleNum };
}

Vector3 operator-(Vector3  const& lVec, Vector3  const& rVec2)
{
	return { lVec.x - rVec2.x,lVec.y - rVec2.y ,lVec.z - rVec2.z };
}








//=======================================[ Vector4 ]=======================================
Vector4 Vector4::operator+=(float const addNum)
{
	x += addNum;
	y += addNum;
	z += addNum;

	return *this;
}
Vector4 Vector4::operator=(float const num)
{
	x = num;
	y = num;
	z = num;

	return *this;
}
//大きさをゲット
float Vector4::GetMagnitutde()
{
	return sqrtf(x * x + y * y + z * z);
}
//正規化ベクトルを返す
Vector4 Vector4::GetNormalized()
{
	if (x == 0.0f && y == 0.0f && z == 0.0f)
	{
		return { 0.0f,0.0f,0.0f,1.0f };
	}

	float inv_mag = 1.0f/ GetMagnitutde();

	Vector4 ret_normalizeVec = { x * inv_mag ,y * inv_mag,z * inv_mag,1.0f };

	return ret_normalizeVec;
}

Vector4 Vector4::GetNormalizedForQuaternion()
{
	if (x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f)
	{
		return { 0.0f,0.0f,0.0f,0.0f };
	}

	float inv_mag = 1.0f/ sqrtf(x * x + y * y + z * z + w * w);

	Vector4 ret_normalizeVec = { x * inv_mag ,y * inv_mag,z * inv_mag,w * inv_mag };

	return ret_normalizeVec;
}

//正規化された内積を計算して返す
float Vector4::GetCos(Vector4 other_)
{
	//正規化する
	Vector4 normalized1 = GetNormalized();
	Vector4 normalized2 = other_.GetNormalized();
	//cos(🍆角)
	float ret_cosTheta =
		normalized1.x * normalized2.x +
		normalized1.y * normalized2.y +
		normalized1.z * normalized2.z;

	return ret_cosTheta;
}

float Vector4::GetDot(Vector4 other_)
{
	float ret_dot = x * other_.x + y * other_.y + z * other_.z;
	return ret_dot;
}

float Vector4::GetCosForQuaternion(Vector4 other_)
{
	//cos(🍆角)
	float ret_cosTheta =
		x * other_.x +
		y * other_.y +
		z * other_.z +
		w * other_.w;

	return ret_cosTheta;
}

//マトリックスとベクタの積
Vector4 Vector4::GetMultiply(Matrix4 src_mat)
{
	Vector4 ret_Vector4;

	ret_Vector4.x = x * src_mat.m[0][0] + y * src_mat.m[1][0] +
		z * src_mat.m[2][0] + w * src_mat.m[3][0];

	ret_Vector4.y = x * src_mat.m[0][1] + y * src_mat.m[1][1] +
		z * src_mat.m[2][1] + w * src_mat.m[3][1];

	ret_Vector4.z = x * src_mat.m[0][2] + y * src_mat.m[1][2] +
		z * src_mat.m[2][2] + w * src_mat.m[3][2];

	ret_Vector4.w = x * src_mat.m[0][3] + y * src_mat.m[1][3] +
		z * src_mat.m[2][3] + w * src_mat.m[3][3];

	return ret_Vector4;
}
//クロス積を返す
Vector4 Vector4::GetCross(Vector4 other)
{
	Vector4 ret_cross;

	ret_cross.x = y * other.z - z * other.y;
	ret_cross.y = z * other.x - x * other.z;
	ret_cross.z = x * other.y - y * other.x;

	return ret_cross;
}

Vector4 operator+(Vector4 const& lVec, Vector4 const& rVec2)
{
	Vector4 ret_vec = { lVec.x + rVec2.x,lVec.y + rVec2.y ,lVec.z + rVec2.z ,1 };

	return ret_vec;
}

Vector4 operator-(Vector4 const& lVec, Vector4 const& rVec2)
{
	Vector4 ret_vec = { lVec.x - rVec2.x,lVec.y - rVec2.y ,lVec.z - rVec2.z ,1 };

	return ret_vec;
}

Vector4 operator*(Vector4 const& dst_vec, float const multipleNum)
{
	Vector4 ret_vec = { dst_vec.x * multipleNum,dst_vec.y * multipleNum ,dst_vec.z * multipleNum ,1 };

	return ret_vec;
}
