#include "vectorAndMatrix.h"
#include <math.h>

const Vector3 V_Vector::kUp = { 0.0f,1.0f,0.0f };
const Vector3 V_Vector::kDown = { 0.0f,-1.0f,0.0f };
const Vector3 V_Vector::kRight = { 1.0f,0.0f,0.0f };
const Vector3 V_Vector::kLeft = { -1.0f,0.0f,0.0f };
const Vector3 V_Vector::kBeyond = { 0.0f,0.0f,1.0f };
const Vector3 V_Vector::kBack = { 0.0f,0.0f,-1.0f };
const Vector3 V_Vector::kZero = { 0.0f,0.0f,0.0f };

////平行投影
//if (type_ == kParallel)
//{
//	projectionMat = Get_Orthographic3D(
//		cameraPos.x - (WINDOW_W * 0.5f), cameraPos.x + (WINDOW_W * 0.5f),
//		cameraPos.y + (WINDOW_H * 0.5f), cameraPos.y - (WINDOW_H * 0.5f),
//		0.1f, 1000.0f);
//}
//
////中心投影
//else if (type_ == kPerspective)
//{
//	float aspectRatio = WINDOW_W / WINDOW_H;
//	projectionMat = Get_PerspectiveFOV(Torima::torimaFOVy, aspectRatio);
//}


Matrix4 Get_VPMat(Matrix4 cameraMat, Matrix4 projectionMat_)
{
	Matrix4 ret_vpMat;

	//ビュー行列作成
	Matrix4 viewMat = cameraMat.GetInversed();
	//viewMat⇔PerspectiveMat
	ret_vpMat = viewMat* projectionMat_;

	return  ret_vpMat;
}

Matrix4 Get_PerspectiveFOV(float fovY_, float aspectRatio_, float nearClip_, float farClip_)
{
	const float cotTheta{ 1.0f / tanf(fovY_ * 0.5f) };
	const float inv_frustumHeight{ 1.0f / (farClip_ - nearClip_) };

	return Matrix4
	{
		cotTheta / aspectRatio_ , 0.0f, 0.0f, 0.0f,
		0.0f, cotTheta, 0.0f, 0.0f,
		0.0f, 0.0f, farClip_ * inv_frustumHeight, 1.0f,
		0.0f, 0.0f, -nearClip_ * farClip_ * inv_frustumHeight, 0.0f,
	};
}
Matrix4 Get_ViewportTransformation3D(
	const float left_, const float top_,
	const float windowWidth_, const float windowHeight_,
	const float minDepth_, const float maxDepth_)
{
	return Matrix4
	{
		windowWidth_ * V_Common::kHalf, 0.0f, 0.0f, 0.0f,
		0.0f, -windowHeight_ * V_Common::kHalf, 0.0f, 0.0f,
		0.0f, 0.0f, maxDepth_ - minDepth_, 0.0f,
		left_ + windowWidth_ * V_Common::kHalf, top_ + windowHeight_ * V_Common::kHalf, minDepth_, 1.0f,
	};
}
Matrix4 Get_Orthographic3D(
	const float l_, const float r_,
	const float t_, const float b_,
	const float zn_, const float zf_)
{
	const float inv_W{ 1.0f / (r_ - l_) };
	const float inv_H{ 1.0f / (t_ - b_) };
	const float inv_D{ 1.0f / (zf_ - zn_) };

	return Matrix4
	{
		2.0f * inv_W, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f * inv_H, 0.0f, 0.0f,
		0.0f, 0.0f, inv_D, 0.0f,
		-(l_ + r_) * inv_W, -(t_ + b_) * inv_H, -zn_ * inv_D, 1.0f,
	};
}

Matrix4 GetQuaternionRotateMat(Vector4 quaternion_)
{
	Vector4 c_ = quaternion_;

	float a00 = (c_.x * c_.x) - (c_.y * c_.y) - (c_.z * c_.z) + (c_.w * c_.w);
	float a01 = (c_.x * c_.y - c_.z * c_.w) * 2.0f;
	float a02 = (c_.x * c_.z + c_.y * c_.w) * 2.0f;

	float a10 = (c_.x * c_.y + c_.z * c_.w) * 2.0f;
	float a11 = -(c_.x * c_.x) + (c_.y * c_.y) - (c_.z * c_.z) + (c_.w * c_.w);
	float a12 = (c_.y * c_.z - c_.x * c_.w) * 2.0f;

	float a20 = (c_.x * c_.z - c_.y * c_.w) * 2.0f;
	float a21 = (c_.y * c_.z + c_.x * c_.w) * 2.0f;
	float a22 = -(c_.x * c_.x) - (c_.y * c_.y) + (c_.z * c_.z) + (c_.w * c_.w);


	return Matrix4
	{
		a00, a10, a20, 0.0f,
		a01, a11, a21, 0.0f,
		a02, a12, a22, 0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};

}


Matrix4 GetScaleMat(Vector3 scale_)
{
	return
	{
		scale_.x,0.0f,0.0f,0.0f,
		0.0f,scale_.y,0.0f,0.0f,
		0.0f,0.0f,scale_.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
}

Matrix4 GetTranslateMat(Vector3 translate_)
{
	//TranslateMat
	return
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		translate_.x,translate_.y,translate_.z,1.0f
	};

}

Matrix4 Get_SQrTMat3D(const Vector3& scale_, Vector4 quaternion_, const Vector3& translate_)
{
	Matrix4 ret;

	//SRT行列の作成
	Matrix4 scaleMat = GetScaleMat(scale_);
	Matrix4 rotateMat = GetQuaternionRotateMat(quaternion_);
	//TranslateMat
	Matrix4 translateMat = GetTranslateMat(translate_);

	//S⇔R
	ret = scaleMat.Multiply(rotateMat);
	//SR⇔T
	ret = ret.Multiply(translateMat);

	return ret;		//SRT

}

Matrix4 Get_SRTMat3D(const Vector3& scale_, const Vector3& rotateTheta_, const Vector3& translate_)
{
	Matrix4 ret_mat;

	//回転角をradianに変換
	static float const degreeConverter = V_Common::kPi / 180.0f;
	Vector3 rotateRad = rotateTheta_ * degreeConverter;

	//3つの回転軸に対応した行列を作成
	//角度を三角関数に代入して返すラムダ式
	auto substituteRad = [](float rotateRad_) {
		Vector3 ret_vec = { cosf(rotateRad_),sinf(rotateRad_),1.0f };
		return ret_vec; };

	//x軸
	Vector3 circularFunc = substituteRad(rotateRad.x);

	Matrix4 rotateMatX(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, circularFunc.x, circularFunc.y, 0.0f,
		0.0f, -circularFunc.y, circularFunc.x, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	//y軸
	circularFunc = substituteRad(rotateRad.y);
	Matrix4 rotateMatY =
	{
		circularFunc.x,0.0f,-circularFunc.y,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		circularFunc.y,0.0f,circularFunc.x,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	//z軸
	circularFunc = substituteRad(rotateRad.z);
	Matrix4 rotateMatZ =
	{
		circularFunc.x,circularFunc.y,0.0f,0.0f,
		-circularFunc.y,circularFunc.x,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	//3つの回転マトリックスを合成する
	Matrix4 concentratedRotateMat = rotateMatX.Multiply(rotateMatY.Multiply(rotateMatZ));

	//SRT行列の作成
	//ScaleMatrix
	Matrix4 scaleMat =
	{
		scale_.x,0.0f,0.0f,0.0f,
		0.0f,scale_.y,0.0f,0.0f,
		0.0f,0.0f,scale_.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	//TranslateMat
	Matrix4 translateMat =
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		translate_.x,translate_.y,translate_.z,1.0f
	};

	//S⇔R
	ret_mat = scaleMat.Multiply(concentratedRotateMat);
	//SR⇔T
	ret_mat = ret_mat.Multiply(translateMat);

	return ret_mat;		//SRT

}//SRT行列の作成・取得

Matrix4 Get_STRMat3D(const Vector3& scale_, const Vector3& movementTheta_, const Vector3& translate_)
{
	Matrix4 ret_mat;

	//回転角をradianに変換
	static float const degreeConverter = V_Common::kPi / 180.0f;
	Vector3 rotateRad = movementTheta_ * degreeConverter;

	//3つの回転軸に対応した行列を作成
	//角度を三角関数に代入して返すラムダ式
	auto substituteRad = [](float movementTheta_) {
		Vector3 ret_vec = { cosf(movementTheta_),sinf(movementTheta_),1.0f };
		return ret_vec; };

	//x軸
	Vector3 circularFunc = substituteRad(rotateRad.x);
	Matrix4 movementMatX =
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,circularFunc.x,circularFunc.y,0.0f,
		0.0f,-circularFunc.y,circularFunc.x,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	//y軸
	circularFunc = substituteRad(rotateRad.y);
	Matrix4 movementMatY =
	{
		circularFunc.x,0.0f,-circularFunc.y,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		circularFunc.y,0.0f,circularFunc.x,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	//z軸
	circularFunc = substituteRad(rotateRad.z);
	Matrix4 movementMatZ =
	{
		circularFunc.x,circularFunc.y,0.0f,0.0f,
		-circularFunc.y,circularFunc.x,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	//3つの回転マトリックスを合成する
	Matrix4 concentratedRotateMat = movementMatX.Multiply(movementMatY.Multiply(movementMatZ));

	//SRT行列の作成
	//ScaleMatrix
	Matrix4 scaleMat =
	{
		scale_.x,0.0f,0.0f,0.0f,
		0.0f,scale_.y,0.0f,0.0f,
		0.0f,0.0f,scale_.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	//TranslateMat
	Matrix4 translateMat =
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		translate_.x,translate_.y,translate_.z,1.0f
	};

	//S⇔T
	ret_mat = scaleMat.Multiply(translateMat);
	//ST⇔R
	ret_mat = ret_mat.Multiply(concentratedRotateMat);

	return ret_mat;		//STR

}

Matrix3 Get_SRTMat2D(const float& delta_scaleX_, const float& delta_scaleY_, const float& delta_rotateTheta_,
	const Vector3& delta_pos_)
{
	Matrix3 ret_mat;

	//回転角をradianに変換
	static float const degreeConverter = V_Common::kPi / 180.0f;
	float delta_rotateRad = delta_rotateTheta_ * degreeConverter;
	float cosT = cosf(delta_rotateRad);
	float sinT = sinf(delta_rotateRad);


	Matrix3 rotateMat =
	{
		cosT,sinT,0.0f,
		-sinT,cosT,0.0f,
		0.0f,0.0f,1.0f,
	};

	//SRT行列の作成
	//ScaleMatrix
	Matrix3 scaleMat =
	{
		delta_scaleX_,0.0f,0.0f,
		0.0f,delta_scaleY_,0.0f,
		0.0f,0.0f,1.0f
	};

	//TranslateMat
	Matrix3 translateMat =
	{
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		delta_pos_.x,delta_pos_.y,1.0f
	};

	//S⇔R
	ret_mat = scaleMat.GetMultiply(&rotateMat);
	//ST⇔R
	ret_mat = ret_mat.GetMultiply(&translateMat);

	return ret_mat;		//STR

}//SRT行列の作成・取得


Vector4 GetPerpendiculer(Vector4 point_)
{
	Vector4 ret_vec;

	if (point_.x != 0.0f || point_.y != 0.0f)
	{
		ret_vec = { -point_.y ,point_.x,0.0f,1.0f };
	}

	else
	{
		ret_vec = { 0.0f ,-point_.z,point_.y ,1.0f };
	}

	return ret_vec;
}

Vector3 ConvertToRectangularCoordinate(Vector3& sphereCoord_)
{
	float tmp1 = sphereCoord_.x * sinf(sphereCoord_.z);

	Vector3 ret =
	{
		tmp1* sinf(sphereCoord_.y),
		sphereCoord_.x * cosf(sphereCoord_.z),
		tmp1* cosf(sphereCoord_.y)
	};

	return ret;
}

Vector4 GetQuaternion(Vector3 v_, float deltaTheta_)
{
	float const degreeConverter = V_Common::kPi / 180.0f;
	float radian = deltaTheta_ * degreeConverter;
	Vector3 buff = v_.GetNormalized();
	float halfRad = radian * V_Common::kHalf;
	float sinNum = sinf(halfRad);

	Vector4 quaternion = { buff.x * sinNum,buff.y * sinNum
		,buff.z * sinNum ,cosf(halfRad) };

	return quaternion;
}

Vector4 GetCompositeQuaternion(Vector4 q_, Vector4 p_)
{

	Matrix4 compositeMatrix =
	{
		q_.w , q_.z, -q_.y, -q_.x,
		-q_.z , q_.w, q_.x, -q_.y,
		q_.y , -q_.x, q_.w, -q_.z,
		q_.x , q_.y, q_.z, q_.w,
	};


	return p_.GetMultiply(compositeMatrix);
}

Vector2 ConvertToScreen(Vector3 dst_, Matrix4& viewProjectionMat_)
{
	Vector4 tmp = { dst_.x,dst_.y,dst_.z,1.0f };

	Matrix4 VPV_mat = viewProjectionMat_.Multiply(Get_ViewportTransformation3D());
	tmp = tmp.GetMultiply(VPV_mat);
	float const inv_w = 1.0f / tmp.w;

	//チェック
	tmp = { tmp.x * inv_w ,tmp.y * inv_w  ,tmp.z * inv_w ,1.0f };

	return Vector2{ tmp.x ,tmp.y };
}
