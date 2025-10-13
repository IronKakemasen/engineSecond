#pragma once

//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//Matrix3x3　　　　 　　　　　　　　　　　　　　　　　　　　□
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
struct Matrix3
{
	float m[3][3] =
	{
		{1.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f},
		{0.0f,0.0f,1.0f},
	};

	Matrix3 operator+=(Matrix3 other);
	Matrix3 operator-=(Matrix3 other);

	Matrix3()
	{
	};

	Matrix3(float zero_);
	Matrix3(float a0, float a1, float a2, float a3, float a4, float a5,
		float a6, float a7, float a8);

	Matrix3 GetMultiply(Matrix3* const other);
};


//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//Matrix4x4　　　　 　　　　　　　　　　　　　　　　　　　　□
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
struct Matrix4
{

	float m[4][4] =
	{
		{1.0f,0.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f,0.0f},
		{0.0f,0.0f,1.0f,0.0f},
		{0.0f,0.0f,0.0f,1.0f},
	};

	Matrix4 operator+=(Matrix4 other);
	Matrix4 operator-=(Matrix4 other);

	Matrix4()
	{
	};

	Matrix4(float zero_);
	Matrix4(float a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9,
		float a10, float a11, float a12, float a13, float a14, float a15);

	//マトリックス同士の積
	Matrix4 Multiply(Matrix4 const& other);

	//****************************************
	//https://suzulang.com/inverse-matrix-44/
	//****************************************
	//4x4行列の行列式を返す関数
	float GetDeterminant4x4();
	//4x4行列の逆行列を取得
	Matrix4 GetInversed();

};



Matrix4 operator*(Matrix4& lMat_, Matrix4& rMat_);
