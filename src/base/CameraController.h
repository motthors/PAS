#ifndef _CAMERA_CONTROLLER_H_
#define _CAMERA_CONTROLLER_H_
//////////////////////////////////////
/*		カメラコントローラクラス
カメラクラスを複数保持し操作する
/////////////////////////////////////*/

//class CameraController;

#include"IncludeAll.h"
#include"Camera.h"
//#include"_BaseObject.h"


class CameraController{

	int CameraNum;		// カメラの個数

	float WindowWidth;	//レンダーターゲットのサイズ
	float WindowHeight;

	Camera** ppCamera;
	int CameraIndex;

	void Release();

public:
	CameraController();
	~CameraController();

	void Init(int CamNum, DXDEVICE* pdev, DebugCommentManager* pdebm);

	// 総プレイ人数とカメラ番号からAspectRatioとViewportの設定
	void SetViewPort(int playerNum);

	// 操作するカメラの変更
	void ChangeCamera(int num);
	
	//pos,dir,upを設定
	void SetView(vec *pos, vec *dir, vec *up);

	//射影変換行列をセット
	void SetPers(float fovYRad, float znear=0.1f, float zfar=10000000.0f);

	//操作したいカメラのポインタを取得
	Camera* GetCameraPointer(int idx);
	Camera* GetCameraPointer();

	//カメラのビューポート設定？
	void SetWindowSize(float width, float height);

};

#endif
