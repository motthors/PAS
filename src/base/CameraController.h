#ifndef _CAMERA_CONTROLLER_H_
#define _CAMERA_CONTROLLER_H_
//////////////////////////////////////
/*		�J�����R���g���[���N���X
�J�����N���X�𕡐��ێ������삷��
/////////////////////////////////////*/

//class CameraController;

#include"IncludeAll.h"
#include"Camera.h"
//#include"_BaseObject.h"


class CameraController{

	int CameraNum;		// �J�����̌�

	float WindowWidth;	//�����_�[�^�[�Q�b�g�̃T�C�Y
	float WindowHeight;

	Camera** ppCamera;
	int CameraIndex;

	void Release();

public:
	CameraController();
	~CameraController();

	void Init(int CamNum, DXDEVICE* pdev, DebugCommentManager* pdebm);

	// ���v���C�l���ƃJ�����ԍ�����AspectRatio��Viewport�̐ݒ�
	void SetViewPort(int playerNum);

	// ���삷��J�����̕ύX
	void ChangeCamera(int num);
	
	//pos,dir,up��ݒ�
	void SetView(vec *pos, vec *dir, vec *up);

	//�ˉe�ϊ��s����Z�b�g
	void SetPers(float fovYRad, float znear=0.1f, float zfar=10000000.0f);

	//���삵�����J�����̃|�C���^���擾
	Camera* GetCameraPointer(int idx);
	Camera* GetCameraPointer();

	//�J�����̃r���[�|�[�g�ݒ�H
	void SetWindowSize(float width, float height);

};

#endif
