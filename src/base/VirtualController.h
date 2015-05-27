#ifndef _VIRTUALCONTROLLER_H_
#define _VIRTUALCONTROLLER_H_

/////////////////////////////
/* 仮想コントローラー
すべてのゲーム上のオブジェクトはコレを参照する
ゲーム部分とインプットをつなぐ役割
コンフィグもやる
////////////////////////////*/


/* プログラム内でのボタン順
0　△
1　○
2　×
3　□
4 L1
5 R1
6 L2
7 R2
9 8 L3
10 9 R3
11 10 start
8 11 select
*/
#include"IncludeAll.h"
#include"Input.h"

#define _VC_TRIANGLE	0
#define _VC_CIRCLE		1
#define _VC_CROSS		2
#define _VC_SQUARE		3
#define _VC_L1			4
#define _VC_R1			5
#define _VC_L2			6
#define _VC_R2			7
#define _VC_L3			8
#define _VC_R3			9
#define _VC_START		10
#define _VC_SELECT		11

#define _VC_STICK_LY	0
#define _VC_STICK_LX	1
#define _VC_STICK_RY	2
#define _VC_STICK_RX	3
#define _VC_BUTTON_LR	4
#define _VC_STICK_NO	0xFF

#define playerNumConv ConvControllerID[playerNum]

class virtualController{

	Input *pInput;

	// 追加コントローラ変換配列
	short ConvControllerID[PadNum];

	// ボタンの番号を格納
	BYTE convBotton[128];
	
	// スティックの種類
	BYTE convStick[3];		// 0:pitch 1:roll 2:yaw
	BYTE convCamera[2];		// 0:Y 1:X 
	int  reverseFlag[4];	// 0:pitch 1:pitch 2:cameraY 3:cameraX

	FLOAT stickControl(int flag, BYTE playerNum);

public:
	virtualController();
	~virtualController();
	void Load();
	void setInput(Input* p);

	void reset();

	// メニュー用ボタンセット
	BYTE OK(BYTE playerNum=0);
	BYTE Cancel(BYTE playerNum=0);
	BYTE Up(BYTE playerNum=0);
	BYTE Down(BYTE playerNum=0);
	BYTE Left(BYTE playerNum=0);
	BYTE Right(BYTE playerNum=0);
	
	// ゲーム用ボタンセット
	BYTE Accel(BYTE playerNum=0);
	BYTE Brake(BYTE playerNum=0);
	BYTE UseItem(BYTE playerNum=0);
	BYTE FaceFoward(BYTE playerNum=0);
	BYTE CamChangeNear(BYTE playerNum=0);
	BYTE CamChangeFar(BYTE playerNum=0);
	BYTE ChangeDriveMode(BYTE playerNum);
	
	// スティックセット
	FLOAT Pitch(BYTE playerNum=0);
	FLOAT Roll(BYTE playerNum=0);
	FLOAT Yaw(BYTE playerNum=0);
	FLOAT Camera(int XYflag, BYTE playerNum=0);// Y=0,X=1


	//直取りシリーズ
	FLOAT LX(BYTE playerNum=0);
	FLOAT LY(BYTE playerNum=0);
	FLOAT RX(BYTE playerNum=0);
	FLOAT RY(BYTE playerNum=0);
	BYTE Button(int bnum, BYTE playerNum=0);
	BYTE ButtonN(int bnum, BYTE playerNum=0);
	BYTE CrossD(int direct, BYTE playerNum=0);

	//その他いろいろ
	int JoinNewPlayer(int MaxPlayer);

	Input* Debug_GetInputPointer(){ return pInput; }
};

typedef virtualController VCon;

#endif