#ifndef _MENUMANAGER_BASE_H_
#define _MENUMANAGER_BASE_H_

//////////////////////////////////////////////////
//	 ���j���[�\�������Ǘ�
//	�\����J�ڏ����Ȃǂ͂��Ȃ����ۃN���X

#include"../base/IncludeAll.h"
#include"MenuManager_Parts.h"
#include"../base/DebugCommentManager.h"
#include"../base/VirtualController.h"

class MenuManagerBase{

	MMParts_Base* MenuArr;
	MMParts_Base* iterator;
	int AddModeFlag;
	int CurrentLayer=0;	//�ݒ蒆�̃��j���[�̃��C���[�� Root:0

	enum enumAddModeFlag{
		_MM_BROS_,
		_MM_CHILD_,
		_MM_PARENT_,
		_MM_ROOT_
	};

protected:
	//�p���悩��g������֐�
	MMParts_Base* GetIterator(){ return iterator; }
	MMParts_Base* GetMenuArr(){ return MenuArr; }
	//void MoveIterator_Up();
	bool MoveIterator_Down();
	bool MoveIterator_Select();
	bool MoveIterator_Cancel();
	void MoveIterator_Reset();
	int GetChildNum();

public:
	MenuManagerBase();
	virtual ~MenuManagerBase();

	void AddMenuStruct(int Layer, MMParts_Base* pMMP, TCHAR* name);
	void SetParent();
	void EndSetUp();

};

#endif