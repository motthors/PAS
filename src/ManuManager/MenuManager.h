#ifndef _MENUMANAGER_H_
#define _MENUMANAGER_H_

///////////////////////////////////////////////////////
//	�\�������łȂ��\���Ȃǂ̊Ǘ����s���N���X

#include"MenuManagerBase.h"
#include"../base/VirtualController.h"
#include"../base/DataBox.h"
#include"../2DDrawer/2DDrawManager.h"
#include"MMParts_Draw.h"

class MenuManager : public MenuManagerBase{
	virtualController*	pInput;
	DataBox*			pDataBox;
	My2DDrawManager*	p2DDrawer;	// MMParts�ɂ�����Drawer
	MMParts_Base*		mmpClicked = nullptr;
	MMParts_Base*		mmpMenuDrag = nullptr; // �h���b�O������True
	LONG				DragX = 0;
	LONG				DragY = 0;	// �h���b�O�����p 1�t���[���O�̃s�N�Z�����W�l
	//test 
	int		idx;
	BYTE	InOutFlag = 0; // 0:Disapp 1:Appear
	bool	MouseOnMenuFlag;	// ���j���[�𑀍삵�Ă�����True
	float	AspectDistortion;	// �c�����̂䂪�ݕ␳�p�ϐ�(�����蔻�莞��Y���ǂꂾ���Ԃ���) MMParts_Draw�̓����蔻��Ɏg�p
public:
	MenuManager(virtualController* pvc, DataBox* pdb, My2DDrawManager* p2d);
	~MenuManager(){}

	// MenuManager��AddMenuStruct()�̃��b�s���O�F�p�r�ʂ̃��j���[����
	void AddMenuStruct_Root(TCHAR* name);
	void AddMenuStruct_Folder(int Layer, TCHAR* name);
	void AddMenuStruct_Leaf(int Layer, TCHAR* name);
	// function<void()>�ɂ� std::bind(&ClassName::Func,ClassInstancePointer)������悤��
	void AddMenuStruct_Func(int Layer, TCHAR* name, std::function<void()> func);
	
	// �����ς������̂̓_�T�����ǂ������Ȃ��E�E�E
	void AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float* pVariable, 
								float MinVar, float MaxVar, int IntVariableflag = 0);
	void AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float* pVariable,
								float* pMin, float MaxVar, int IntVariableflag = 0);
	void AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float* pVariable,
								float MinVar, float* pMax, int IntVariableflag = 0);
	void AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float* pVariable,
								float* pMin, float* pMax, int IntVariableflag = 0);

	// �_�T�����ppVariable�p
	void AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float** ppVariable,
								float MinVar, float MaxVar, int IntVariableflag = 0);
	void AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float** ppVariable,
								float* pMin, float MaxVar, int IntVariableflag = 0);
	void AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float** ppVariable,
								float MinVar, float* pMax, int IntVariableflag = 0);
	void AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float** ppVariable,
								float* pMin, float* pMax, int IntVariableflag = 0);


	// SlideBar_Color()�͌Z��𐶐������Ȃ��悤�ɂ��邱��
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

	void Move();	// �߂�l��True�Ȃ烁�j���[���쒆
	void Draw();
	void Draw(DebugCommentManager* p);
};

#endif