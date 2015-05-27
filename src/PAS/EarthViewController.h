#ifndef _EARTH_VIEW_CONTROLLER_H_
#define _EARTH_VIEW_CONTROLLER_H_


#include"../base/IncludeAll.h"
#include"../base/CameraController.h"

class EarthViewController{

	CameraController* m_pCam;

public:
	vec3 m_vec3Ground;
	vec3 m_vec3CamDir;
	float m_LengthGtoC;
	float m_LengthCDir;
	float m_AngleHeight;

	EarthViewController(CameraController* p);
	~EarthViewController();

	// controller
	void MoveGround(float x, float y);
	void MoveDirection(float diff);
	void MoveAngle(float diff);
	void MoveLength(float diff);

	// Get
	vec GetGround(float len);
	vec GetCamDir();
	float GetLength(){ return m_LengthCDir; }
};

#endif