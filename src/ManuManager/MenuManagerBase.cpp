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
//新しく作られたメニューを適当な位置に配置する(固有の値は呼び出し元でやる)
{
	//Layer処理
	int DifferenceLayer = Layer - CurrentLayer;
	//Root(=0)より上には設定できない
	if (Layer < 0)throw - 1;
	//1つ奥のLayer設定要求ならChildへ
	if (DifferenceLayer == 1) AddModeFlag = _MM_CHILD_;
	//Root用処理
	else if (Layer == 0) AddModeFlag = _MM_ROOT_;
	//同じLayer設定要求ならBrotherへ
	else if (DifferenceLayer == 0) AddModeFlag = _MM_BROS_;
	//2つ以上奥のレイヤーに設定要求ならエラー
	else if (DifferenceLayer >= 2)throw - 1;
	//上の階層へ戻る
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
//成功したらture、失敗したらfalse
{
	if (iterator->Brother)iterator = iterator->Brother;
	else return false;
	return true;
}
bool MenuManagerBase::MoveIterator_Select()
//成功したらture、失敗したらfalse
{
	if (iterator->Child)iterator = iterator->Child;
	else return false;
	return true;
}
bool MenuManagerBase::MoveIterator_Cancel()
//成功したらture、失敗したらfalse
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
