#ifndef _MATRIX_BOX_H_
#define _MATRIX_BOX_H_
///////////////////////////////////////////
//		MatrixBoxクラス
//描画に使うワールド行列を保持しておく
//マルチスレッド用クラス
//////////////////////////////////////////
#include"IncludeAll.h"
#include"CameraController.h"

#define MAX_OBJNUM		10000
#define MAX_OBJNUM_T	40

//MATBOXは0開始の連番じゃないとダメ
#define MATBOX_SPACE		0
#define MATBOX_SpLOW		1
#define MATBOX_LOW			2
#define MATBOX_NEAR			3
#define MATBOX_SemiTrans	4
#define MATBOX_Machine		5
#define MATBOX_EFFECT		6

#define MATBOX_TOTALNUM	7

typedef struct _MatirxBox{
	mat4		mat;
	int			ObjNum;
	int			TexNum;
	int			MeshIdx;
	_MatirxBox	*parent;
	_MatirxBox	*next;
	_MatirxBox	*prev;
} MatrixBox;

typedef struct _MatirxBoxTime{
	mat4	mat;
	DWORD	Time;
} MatrixBoxTime;

//typedef struct _PointerSet{
//	MatrixBox*	pSuperLow;
//	MatrixBox*	pLow;
//	MatrixBox*	pNear;
//	MatrixBox*	pSemiTransparent;
//	MatrixBox*	pMachine;
//	MatrixBox*	pEffect;
//	MatrixBox*	pSpace;
//} PointerSet;

class MatrixBoxManager{

	CameraController*	pCamera;

	MatrixBox*	pMB[2];
	BYTE		StateIndex;

	MatrixBox* pSet[MATBOX_TOTALNUM];

	MatrixBoxTime* pMBT[2];

	int BoxIdx;
	int TimeIdx;
	MatrixBox* GetEndOfPtr(MatrixBox* ptr);

	void ConectPointer(int FLAG);
	void TransByPosidex(mat4* mat, POSINDEX *idx);

public:
	MatrixBoxManager();
	MatrixBoxManager(int ObjNum);
	void ConstructInit();
	~MatrixBoxManager();
	void SetCameraPointer(CameraController* pcam){pCamera = pcam;}

	void BeginSet();
	void ChangeStateIndex();
	mat4* GetNowMatrixPointer();
	void SetMatrix( POSINDEX *posidx, int objnum, int texnum, int FLAG=MATBOX_NEAR, int meshidx=-1);
	void SetMatrix( mat4* mat, POSINDEX *posidx, int objnum, int texnum, int FLAG=MATBOX_NEAR, int meshidx=-1);
	void SetTime(mat4 *mat, DWORD Time);
	mat4* ConvertBillBoardMatrix(mat4* mat, vec* pos, POSINDEX* idx, float scale=1.f, bool NoProj=true);
	
	MatrixBox** GetPointerSet(){return pSet;}
};

#endif