#pragma once
#include "../../math/matrix/matrix.h"
#include "../../math/vector/vector.h"
#include "../../commonVariables.h"


namespace V_Vector
{
	extern const Vector3 kUp;
	extern const Vector3 kDown;
	extern const Vector3 kRight;
	extern const Vector3 kLeft;
	extern const Vector3 kBeyond;
	extern const Vector3 kBack;
	extern const Vector3 kZero;

}

//SRT行列の作成・取得
Matrix4 Get_SRTMat3D(const Vector3& scale_, const Vector3& rotateTheta_,
	const Vector3& translate_);
//STR行列の作成・取得
Matrix4 Get_STRMat3D(const Vector3& scale_, const Vector3& movementTheta_,
	const Vector3& translate_);
//正射影行列3D
Matrix4 Get_Orthographic3D(
	const float l_, const float r_,
	const float t_, const float b_,
	const float zn_ = 0.1f, const float zf_ = 100.0f);
//ビューポート変換3D
Matrix4 Get_ViewportTransformation3D(
	const float left_ = 0.0f, const float top_ = 0.0f,
	const float windowWidth_ = V_Common::kWindow_W, const float windowHeight_ = V_Common::kWindow_H,
	const float minDepth_ = 0.0f, const float maxDepth_ = 1.0f);
//透視投影行列
Matrix4 Get_PerspectiveFOV(float fovY_, float aspectRatio_, float nearClip_ = 0.1f, float farClip_ = 1000.0f);
//VP行列
Matrix4 Get_VPMat(Matrix4 cameraMat, Matrix4 projectionMat_);
//STR行列の作成・取得
Matrix3 Get_SRTMat2D(const float& delta_scaleX_, const float& delta_scaleY_, const float& delta_rotateTheta_,
	const Vector3& delta_pos_);
Matrix4 GetQuaternionRotateMat(Vector4 quaternion_);
Vector4 GetQuaternion(Vector3 v_, float deltaTheta_);
Matrix4 Get_SQrTMat3D(const Vector3& scale_, Vector4 quaternion_, const Vector3& translate_);
Vector4 GetCompositeQuaternion(Vector4 p_, Vector4 q_);
Matrix4 GetScaleMat(Vector3 scale_);
Matrix4 GetTranslateMat(Vector3 translate_);
Vector4 GetPerpendiculer(Vector4 point_);
Vector3 ConvertToRectangularCoordinate(Vector3& sphereCoord_);
Vector2 ConvertToScreen(Vector3 dst_, Matrix4& viewProjectionMat_);
