#ifndef _MENUMANAGER_H_
#define _MENUMANAGER_H_

///////////////////////////////////////////////////////
//	構造だけでなく表示などの管理を行うクラス

#include"MenuManagerBase.h"
#include"../base/VirtualController.h"
#include"../base/DataBox.h"
#include"../2DDrawer/2DDrawManager.h"
#include"MMParts_Draw.h"

class MenuManager : public MenuManagerBase{
	virtualController*	pInput;
	DataBox*			pDataBox;
	My2DDrawManager*	p2DDrawer;	// MMPartsにあげるDrawer
	MMParts_Base*		mmpClicked = nullptr;
	MMParts_Base*		mmpMenuDrag = nullptr; // ドラッグしたらTrue
	LONG				DragX = 0;
	LONG				DragY = 0;	// ドラッグ処理用 1フレーム前のピクセル座標値
	//test 
	int		idx;
	BYTE	InOutFlag = 0; // 0:Disapp 1:Appear
	bool	MouseOnMenuFlag;	// メニューを操作していたらTrue
	float	AspectDistortion;	// 縦方向のゆがみ補正用変数(当たり判定時にYをどれだけつぶすか) MMParts_Drawの当たり判定に使用
public:
	MenuManager(virtualController* pvc, DataBox* pdb, My2DDrawManager* p2d);
	~MenuManager(){}

	// MenuManagerのAddMenuStruct()のラッピング：用途別のメニュー生成
	void AddMenuStruct_Root(TCHAR* name);
	void AddMenuStruct_Folder(int Layer, TCHAR* name);
	void AddMenuStruct_Leaf(int Layer, TCHAR* name);
	// function<void()>には std::bind(&ClassName::Func,ClassInstancePointer)を入れるように
	void AddMenuStruct_Func(int Layer, TCHAR* name, std::function<void()> func);
	
	// いっぱい書くのはダサいけどしかたない・・・
	void AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float* pVariable, 
								float MinVar, float MaxVar, int IntVariableflag = 0);
	void AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float* pVariable,
								float* pMin, float MaxVar, int IntVariableflag = 0);
	void AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float* pVariable,
								float MinVar, float* pMax, int IntVariableflag = 0);
	void AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float* pVariable,
								float* pMin, float* pMax, int IntVariableflag = 0);

	// ダサいやつのppVariable用
	void AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float** ppVariable,
								float MinVar, float MaxVar, int IntVariableflag = 0);
	void AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float** ppVariable,
								float* pMin, float MaxVar, int IntVariableflag = 0);
	void AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float** ppVariable,
								float MinVar, float* pMax, int IntVariableflag = 0);
	void AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float** ppVariable,
								float* pMin, float* pMax, int IntVariableflag = 0);


	// SlideBar_Color()は兄弟を生成させないようにすること
	void AddMenuStruct_SlideBar_Color(int Layer, TCHAR* name, 
								vec3* pVariable, float MinVar, float MaxVar);
	

	MMParts_Draw* GetIterator(){ return (MMParts_Draw*)MenuManagerBase::GetIterator(); }
	MMParts_Draw* GetMenuArr(){ return  (MMParts_Draw*)MenuManagerBase::GetMenuArr(); }

	void SetClickedMenu();
	void ClickDecision();
	void ShiftMenuStructs();
	void TakeInAndOutMenu();
	void SetRootMenuPos(float* xFix, float* yFix);
	bool CheckOnMouse();

	void Move();	// 戻り値がTrueならメニュー操作中
	void Draw();
	void Draw(DebugCommentManager* p);
};

#endif