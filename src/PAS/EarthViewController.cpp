#include"EarthViewController.h"


EarthViewController::EarthViewController(CameraController* p)
{
	m_pCam = p;
	m_vec3Ground = vec3(0, 1, 0);
	m_vec3CamDir = vec3(0, 0, 1);
	m_LengthGtoC = 0.f;
	m_LengthCDir = 0.f;
	m_AngleHeight = 0.f;
}
EarthViewController::~EarthViewController(){}


void EarthViewController::MoveGround(float x, float y)
{
	vec t_vecGround = XMLoadFloat3(&m_vec3Ground);
	vec t_vecCamDir = XMLoadFloat3(&m_vec3CamDir);

	mat matRota;
	Camera* pc = m_pCam->GetCameraPointer();
	vec dir = XMLoadFloat3(pc->GetCameraDir());
	vec pit = XMLoadFloat3(pc->GetCameraUp());
	pit = XMVector3Cross(pit, dir);
	// dir‰ñ“]
	matRota = XMMatrixRotationAxis(dir, x);
	t_vecGround = XMVector3Transform(t_vecGround, matRota);
	t_vecCamDir = XMVector3Transform(t_vecCamDir, matRota);
	// pitch‰ñ“]
	matRota = XMMatrixRotationAxis(pit, y);
	t_vecGround = XMVector3Transform(t_vecGround, matRota);
	t_vecCamDir = XMVector3Transform(t_vecCamDir, matRota);
	
	XMStoreFloat3(&m_vec3Ground, t_vecGround);
	XMStoreFloat3(&m_vec3CamDir, t_vecCamDir);
}


void EarthViewController::MoveDirection(float diff)
{
	mat matRota;
	vec dirCam = XMLoadFloat3(&m_vec3CamDir);
	vec axis = XMLoadFloat3(&m_vec3Ground);
	matRota = XMMatrixRotationAxis(axis, diff);
	dirCam = XMVector3Transform(dirCam, matRota);
	XMStoreFloat3(&m_vec3CamDir, dirCam);
}

void EarthViewController::MoveAngle(float diff)
{
	m_AngleHeight += diff;
	if (m_AngleHeight <= 0)m_AngleHeight = 0;
	else if (m_AngleHeight >hPHI - 0.1f)m_AngleHeight = hPHI - 0.1f;
}


void EarthViewController::MoveLength(float diff)
{
	m_LengthCDir += diff * 17.f;
	if (m_LengthCDir <= 0.001f)m_LengthCDir = 0.001f;
	else if (m_LengthCDir > 20000.f)m_LengthCDir = 20000.f;
}


vec EarthViewController::GetGround(float len)
{
	vec vGround = XMLoadFloat3(&m_vec3Ground);
	return vGround * len;
}

vec EarthViewController::GetCamDir()
{
	vec vdir = XMLoadFloat3(&m_vec3CamDir);
	vec vGround = XMLoadFloat3(&m_vec3Ground);
	vec cross = XMVector3Cross(vdir, vGround);
	
	mat matRota;
	matRota = XMMatrixRotationAxis(cross, -m_AngleHeight);
	vdir = XMVector3Transform(vdir, matRota);
	return vdir;

}
