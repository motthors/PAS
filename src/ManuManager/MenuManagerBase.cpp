#include"MenuManagerBase.h"


MenuManagerBase::MenuManagerBase()
{
	MenuArr = nullptr;
	iterator = MenuArr;
	CurrentLayer = 0;
}

MenuManagerBase::~MenuManagerBase()
{
	SAFE_DELETE(MenuArr);
}


void MenuManagerBase::AddMenuStruct(int Layer, MMParts_Base* pMMP, TCHAR* name)
//�V�������ꂽ���j���[��K���Ȉʒu�ɔz�u����(�ŗL�̒l�͌Ăяo�����ł��)
{
	//Layer����
	int DifferenceLayer = Layer - CurrentLayer;
	//Root(=0)����ɂ͐ݒ�ł��Ȃ�
	if (Layer < 0)throw - 1;
	//1����Layer�ݒ�v���Ȃ�Child��
	if (DifferenceLayer == 1) AddModeFlag = _MM_CHILD_;
	//Root�p����
	else if (Layer == 0) AddModeFlag = _MM_ROOT_;
	//����Layer�ݒ�v���Ȃ�Brother��
	else if (DifferenceLayer == 0) AddModeFlag = _MM_BROS_;
	//2�ȏ㉜�̃��C���[�ɐݒ�v���Ȃ�G���[
	else if (DifferenceLayer >= 2)throw - 1;
	//��̊K�w�֖߂�
	else if (DifferenceLayer < 0)
	{
		FOR(-DifferenceLayer)SetParent();
		AddModeFlag = _MM_BROS_;
	}



	switch (AddModeFlag)
	{
	case _MM_CHILD_: 
		iterator->Child = pMMP;
		iterator->Child->Parent = iterator;
		iterator->ChildNum++;
		iterator = iterator->Child;
		break;
	case _MM_BROS_: 
		iterator->Brother = pMMP;
		if (iterator->Parent!=nullptr)iterator->Parent->ChildNum++;
		iterator->Brother->Parent = iterator->Parent;
		iterator = iterator->Brother;
		break;
	case _MM_ROOT_:
		MenuArr = pMMP;
		MenuArr->Parent = nullptr;
		iterator = MenuArr;
		break;
	}
	iterator->SetName(name);
	AddModeFlag = _MM_BROS_;
	CurrentLayer = Layer;
}

void MenuManagerBase::SetParent()
{
	if (iterator == MenuArr)return;
	iterator = iterator->Parent;
}

void MenuManagerBase::EndSetUp()
{
	MoveIterator_Reset();
}

bool MenuManagerBase::MoveIterator_Down()
//����������ture�A���s������false
{
	if (iterator->Brother)iterator = iterator->Brother;
	else return false;
	return true;
}
bool MenuManagerBase::MoveIterator_Select()
//����������ture�A���s������false
{
	if (iterator->Child)iterator = iterator->Child;
	else return false;
	return true;
}
bool MenuManagerBase::MoveIterator_Cancel()
//����������ture�A���s������false
{
	if (iterator->Parent)iterator = iterator->Parent;
	else return false;
	return true;
}
int MenuManagerBase::GetChildNum()
{
	return iterator->ChildNum;
}

void MenuManagerBase::MoveIterator_Reset()
{
	iterator = MenuArr;
}
