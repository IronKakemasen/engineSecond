#pragma once

struct Matrix4;
struct Matrix3;

//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//Vector2  　　　　 　　　　　　　　　　　　　　　　　　　　□
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
struct Vector2
{
	float x = 0.0f; 
	float y = 0.0f;

	Vector2(float entries[2]) : x(entries[0]), y(entries[1]){}
	Vector2(float u_, float v_) : x(u_), y(v_){}
	float GetMagnitutde();
	Vector2 GetNormalized();
	float GetCos(Vector2 other_);
	float GetDot(Vector2 other_);

};

//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//Vector3　　　　 　　　　　　　　　　　　　　　　　　　　  □
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
struct Vector3
{
	float x = 0;
	float y = 0;
	float z = 0;

	Vector3(){}
	Vector3(float entries[3]) : x(entries[0]), y(entries[1]), z(entries[2]){}
	Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_){}

	Vector3 GetMultiply(Matrix4 src_mat);
	// 大きさをゲット
	float GetMagnitutde();
	// 正規化ベクトルを返す
	Vector3 GetNormalized();
	//cosΘを計算して返す
	float GetCos(Vector3 other_);
	//内積を計算して返す
	float GetDot(Vector3 other_);
	Vector3 GetCross(Vector3 other);

};


//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//Vector4  　　　　 　　　　　　　　　　　　　　　　　　　　□
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
struct Vector4
{

public:

	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;

	//============Constractor==============
	Vector4(){}
	Vector4(float x_, float  y_, float  z_, float  w_) : x(x_), y(y_), z(z_), w(w_){}
	Vector4(float  entries[4]) : x(entries[0]), y(entries[1]), z(entries[2]), w(entries[3]){}
	//=====================================

	Vector4 operator+=(float const addNum);
	Vector4 operator=(float const num);

	//大きさをセット
	float GetMagnitutde();
	float GetCos(Vector4 other_);
	//クロス積を返す
	Vector4 GetCross(Vector4 other);
	//正規化ベクトルを返す
	Vector4 GetNormalized();
	//cosを計算して返す
	float GetDot(Vector4 other_);

	Vector4 GetNormalizedForQuaternion();
	float GetCosForQuaternion(Vector4 other_);
	//マトリックスとベクタの積
	Vector4 GetMultiply(Matrix4 src_mat);


};


//==================================Operand============================================================
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓Operand↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
Vector4 operator+(Vector4 const& lVec, Vector4 const& rVec2);
Vector4 operator-(Vector4 const& lVec, Vector4 const& rVec2);
Vector4 operator*(Vector4 const& dst_vec, float const multipleNum);

Vector3 operator*(Vector3 const& dst_vec, float const multipleNum);
Vector3 operator+(Vector3 const& lVec, Vector3 const& rVec2);
Vector3 operator-(Vector3  const& lVec, Vector3  const& rVec2);
bool operator==(Vector3  const& lVec, Vector3  const& rVec2);


Vector2 operator-(Vector2  const& lVec_, Vector2  const& rVec_);
Vector2 operator*(Vector2 const& dst_vec_, float const multipleNum);
Vector2 operator+(Vector2 const& lVec_, Vector2 const& rVec_);
