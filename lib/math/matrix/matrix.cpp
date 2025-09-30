#include "matrix.h"

//[ Matrix3 ]
Matrix3 Matrix3::operator+=(Matrix3 other)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int k = 0; k < 3; ++k)
		{
			m[i][k] += other.m[i][k];
		}
	}

	return *this;
}
Matrix3 Matrix3 ::operator-=(Matrix3 other)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int k = 0; k < 3; ++k)
		{
			m[i][k] -= other.m[i][k];
		}
	}

	return *this;
}

Matrix3::Matrix3(float zero_)
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;
};

Matrix3::Matrix3(float a0, float a1, float a2, float a3, float a4, float a5,
	float a6, float a7, float a8)
{
	m[0][0] = a0; m[0][1] = a1; m[0][2] = a2;
	m[1][0] = a3; m[1][1] = a4; m[1][2] = a5;
	m[2][0] = a6; m[2][1] = a7; m[2][2] = a8;
}

Matrix3 Matrix3::GetMultiply(Matrix3* const other)
{
	Matrix3 ret_mat;

	ret_mat.m[0][0] = m[0][0] * other->m[0][0] + m[0][1] * other->m[1][0] + m[0][2] * other->m[2][0];
	ret_mat.m[0][1] = m[0][0] * other->m[0][1] + m[0][1] * other->m[1][1] + m[0][2] * other->m[2][1];
	ret_mat.m[0][2] = m[0][0] * other->m[0][2] + m[0][1] * other->m[1][2] + m[0][2] * other->m[2][2];
	ret_mat.m[1][0] = m[1][0] * other->m[0][0] + m[1][1] * other->m[1][0] + m[1][2] * other->m[2][0];
	ret_mat.m[1][1] = m[1][0] * other->m[0][1] + m[1][1] * other->m[1][1] + m[1][2] * other->m[2][1];
	ret_mat.m[1][2] = m[1][0] * other->m[0][2] + m[1][1] * other->m[1][2] + m[1][2] * other->m[2][2];
	ret_mat.m[2][0] = m[2][0] * other->m[0][0] + m[2][1] * other->m[1][0] + m[2][2] * other->m[2][0];
	ret_mat.m[2][1] = m[2][0] * other->m[0][1] + m[2][1] * other->m[1][1] + m[2][2] * other->m[2][1];
	ret_mat.m[2][2] = m[2][0] * other->m[0][2] + m[2][1] * other->m[1][2] + m[2][2] * other->m[2][2];

	return ret_mat;
}

//[ Matrix4 ]
Matrix4 Matrix4::operator+=(Matrix4 other)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int k = 0; k < 4; ++k)
		{
			m[i][k] += other.m[i][k];
		}
	}

	return *this;
}
Matrix4 Matrix4::operator-=(Matrix4 other)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int k = 0; k < 4; ++k)
		{
			m[i][k] -= other.m[i][k];
		}
	}

	return *this;
}


Matrix4::Matrix4(float zero_)
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	zero_;
};

Matrix4::Matrix4(float a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9,
	float a10, float a11, float a12, float a13, float a14, float a15)
{
	m[0][0] = a0; m[0][1] = a1; m[0][2] = a2; m[0][3] = a3;
	m[1][0] = a4; m[1][1] = a5; m[1][2] = a6; m[1][3] = a7;
	m[2][0] = a8; m[2][1] = a9; m[2][2] = a10; m[2][3] = a11;
	m[3][0] = a12; m[3][1] = a13; m[3][2] = a14; m[3][3] = a15;
}

Matrix4 Matrix4::Multiply(Matrix4 const& other)
{
	Matrix4 ret_mat;

	ret_mat.m[0][0] = m[0][0] * other.m[0][0] + m[0][1] * other.m[1][0] +
		m[0][2] * other.m[2][0] + m[0][3] * other.m[3][0];
	ret_mat.m[0][1] = m[0][0] * other.m[0][1] + m[0][1] * other.m[1][1] +
		m[0][2] * other.m[2][1] + m[0][3] * other.m[3][1];
	ret_mat.m[0][2] = m[0][0] * other.m[0][2] + m[0][1] * other.m[1][2] +
		m[0][2] * other.m[2][2] + m[0][3] * other.m[3][2];
	ret_mat.m[0][3] = m[0][0] * other.m[0][3] + m[0][1] * other.m[1][3] +
		m[0][2] * other.m[2][3] + m[0][3] * other.m[3][3];

	ret_mat.m[1][0] = m[1][0] * other.m[0][0] + m[1][1] * other.m[1][0] +
		m[1][2] * other.m[2][0] + m[1][3] * other.m[3][0];
	ret_mat.m[1][1] = m[1][0] * other.m[0][1] + m[1][1] * other.m[1][1] +
		m[1][2] * other.m[2][1] + m[1][3] * other.m[3][1];
	ret_mat.m[1][2] = m[1][0] * other.m[0][2] + m[1][1] * other.m[1][2] +
		m[1][2] * other.m[2][2] + m[1][3] * other.m[3][2];
	ret_mat.m[1][3] = m[1][0] * other.m[0][3] + m[1][1] * other.m[1][3] +
		m[1][2] * other.m[2][3] + m[1][3] * other.m[3][3];

	ret_mat.m[2][0] = m[2][0] * other.m[0][0] + m[2][1] * other.m[1][0] +
		m[2][2] * other.m[2][0] + m[2][3] * other.m[3][0];
	ret_mat.m[2][1] = m[2][0] * other.m[0][1] + m[2][1] * other.m[1][1] +
		m[2][2] * other.m[2][1] + m[2][3] * other.m[3][1];
	ret_mat.m[2][2] = m[2][0] * other.m[0][2] + m[2][1] * other.m[1][2] +
		m[2][2] * other.m[2][2] + m[2][3] * other.m[3][2];
	ret_mat.m[2][3] = m[2][0] * other.m[0][3] + m[2][1] * other.m[1][3] +
		m[2][2] * other.m[2][3] + m[2][3] * other.m[3][3];

	ret_mat.m[3][0] = m[3][0] * other.m[0][0] + m[3][1] * other.m[1][0] +
		m[3][2] * other.m[2][0] + m[3][3] * other.m[3][0];
	ret_mat.m[3][1] = m[3][0] * other.m[0][1] + m[3][1] * other.m[1][1] +
		m[3][2] * other.m[2][1] + m[3][3] * other.m[3][1];
	ret_mat.m[3][2] = m[3][0] * other.m[0][2] + m[3][1] * other.m[1][2] +
		m[3][2] * other.m[2][2] + m[3][3] * other.m[3][2];
	ret_mat.m[3][3] = m[3][0] * other.m[0][3] + m[3][1] * other.m[1][3] +
		m[3][2] * other.m[2][3] + m[3][3] * other.m[3][3];

	return ret_mat;

}
float Matrix4::GetDeterminant4x4()
{
	const float a11 = m[0][0];
	const float a12 = m[0][1];
	const float a13 = m[0][2];
	const float a14 = m[0][3];

	const float a21 = m[1][0];
	const float a22 = m[1][1];
	const float a23 = m[1][2];
	const float a24 = m[1][3];

	const float a31 = m[2][0];
	const float a32 = m[2][1];
	const float a33 = m[2][2];
	const float a34 = m[2][3];

	const float a41 = m[3][0];
	const float a42 = m[3][1];
	const float a43 = m[3][2];
	const float a44 = m[3][3];


	return a11 * a22 * a33 * a44 + a11 * a23 * a34 * a42 + a11 * a24 * a32 * a43
		+ a12 * a21 * a34 * a43 + a12 * a23 * a31 * a44 + a12 * a24 * a33 * a41
		+ a13 * a21 * a32 * a44 + a13 * a22 * a34 * a41 + a13 * a24 * a31 * a42
		+ a14 * a21 * a33 * a42 + a14 * a22 * a31 * a43 + a14 * a23 * a32 * a41
		- a11 * a22 * a34 * a43 - a11 * a23 * a32 * a44 - a11 * a24 * a33 * a42
		- a12 * a21 * a33 * a44 - a12 * a23 * a34 * a41 - a12 * a24 * a31 * a43
		- a13 * a21 * a34 * a42 - a13 * a22 * a31 * a44 - a13 * a24 * a32 * a41
		- a14 * a21 * a32 * a43 - a14 * a22 * a33 * a41 - a14 * a23 * a31 * a42;

}//4x4行列の行列式を返す関数
Matrix4 Matrix4::GetInversed()
{
	Matrix4 ret_mat;

	const float a11 = m[0][0];
	const float a12 = m[0][1];
	const float a13 = m[0][2];
	const float a14 = m[0][3];

	const float a21 = m[1][0];
	const float a22 = m[1][1];
	const float a23 = m[1][2];
	const float a24 = m[1][3];

	const float a31 = m[2][0];
	const float a32 = m[2][1];
	const float a33 = m[2][2];
	const float a34 = m[2][3];

	const float a41 = m[3][0];
	const float a42 = m[3][1];
	const float a43 = m[3][2];
	const float a44 = m[3][3];

	ret_mat.m[0][0] = a22 * a33 * a44 + a23 * a34 * a42 + a24 * a32 * a43 - a22 * a34 * a43 - a23 * a32 * a44 - a24 * a33 * a42;
	ret_mat.m[0][1] = a12 * a34 * a43 + a13 * a32 * a44 + a14 * a33 * a42 - a12 * a33 * a44 - a13 * a34 * a42 - a14 * a32 * a43;
	ret_mat.m[0][2] = a12 * a23 * a44 + a13 * a24 * a42 + a14 * a22 * a43 - a12 * a24 * a43 - a13 * a22 * a44 - a14 * a23 * a42;
	ret_mat.m[0][3] = a12 * a24 * a33 + a13 * a22 * a34 + a14 * a23 * a32 - a12 * a23 * a34 - a13 * a24 * a32 - a14 * a22 * a33;
	ret_mat.m[1][0] = a21 * a34 * a43 + a23 * a31 * a44 + a24 * a33 * a41 - a21 * a33 * a44 - a23 * a34 * a41 - a24 * a31 * a43;
	ret_mat.m[1][1] = a11 * a33 * a44 + a13 * a34 * a41 + a14 * a31 * a43 - a11 * a34 * a43 - a13 * a31 * a44 - a14 * a33 * a41;
	ret_mat.m[1][2] = a11 * a24 * a43 + a13 * a21 * a44 + a14 * a23 * a41 - a11 * a23 * a44 - a13 * a24 * a41 - a14 * a21 * a43;
	ret_mat.m[1][3] = a11 * a23 * a34 + a13 * a24 * a31 + a14 * a21 * a33 - a11 * a24 * a33 - a13 * a21 * a34 - a14 * a23 * a31;
	ret_mat.m[2][0] = a21 * a32 * a44 + a22 * a34 * a41 + a24 * a31 * a42 - a21 * a34 * a42 - a22 * a31 * a44 - a24 * a32 * a41;
	ret_mat.m[2][1] = a11 * a34 * a42 + a12 * a31 * a44 + a14 * a32 * a41 - a11 * a32 * a44 - a12 * a34 * a41 - a14 * a31 * a42;
	ret_mat.m[2][2] = a11 * a22 * a44 + a12 * a24 * a41 + a14 * a21 * a42 - a11 * a24 * a42 - a12 * a21 * a44 - a14 * a22 * a41;
	ret_mat.m[2][3] = a11 * a24 * a32 + a12 * a21 * a34 + a14 * a22 * a31 - a11 * a22 * a34 - a12 * a24 * a31 - a14 * a21 * a32;
	ret_mat.m[3][0] = a21 * a33 * a42 + a22 * a31 * a43 + a23 * a32 * a41 - a21 * a32 * a43 - a22 * a33 * a41 - a23 * a31 * a42;
	ret_mat.m[3][1] = a11 * a32 * a43 + a12 * a33 * a41 + a13 * a31 * a42 - a11 * a33 * a42 - a12 * a31 * a43 - a13 * a32 * a41;
	ret_mat.m[3][2] = a11 * a23 * a42 + a12 * a21 * a43 + a13 * a22 * a41 - a11 * a22 * a43 - a12 * a23 * a41 - a13 * a21 * a42;
	ret_mat.m[3][3] = a11 * a22 * a33 + a12 * a23 * a31 + a13 * a21 * a32 - a11 * a23 * a32 - a12 * a21 * a33 - a13 * a22 * a31;

	float det = GetDeterminant4x4();

	if (det != 0.0f)
	{
		float inv_det = 1.0f / det;

		for (int i = 0; i < 4; i++)
		{
			for (int k = 0; k < 4; k++)
			{
				ret_mat.m[i][k] *= inv_det;
			}
		}
	}

	return ret_mat;

}//4x4行列の逆行列を取得



Matrix4 operator*(Matrix4& lMat_, Matrix4& rMat_)
{
	return lMat_.Multiply(rMat_);
}
