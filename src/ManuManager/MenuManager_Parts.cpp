#include"MenuManager_Parts.h"
#include"MMParts_Draw.h"

//////////////////////////////////////////////////////
// Base

MMParts_Base::MMParts_Base()
{
	Name[0] = 0;
	Parent = nullptr;
	Brother = nullptr;
	Child = nullptr;
	ChildNum = 0;
}

MMParts_Base::~MMParts_Base()
{
	SAFE_DELETE(Child);
	SAFE_DELETE(Brother);
}

void MMParts_Base::SetName(TCHAR* name)
{
	_tcscpy_s(Name, 10, name);
}


//////////////////////////////////////////////////////
// Root

MMParts_Root::MMParts_Root(){}

MMParts_Root::~MMParts_Root(){}

void MMParts_Root::Appear()
{
	Active();
	plaSize->SetBaseValue(1.0f);
	//SetChildPos
	auto itr = (MMParts_Draw*)Child;
	FOR(ChildNum)
	{
		itr->Appear();
		itr->SetPos(x_texel, y_texel, 0.3*sin(twoPHI*i / (float)ChildNum)*AspectDistortion, 0.3*cos(twoPHI*i / (float)ChildNum), AspectDistortion);
		itr = (MMParts_Draw*)itr->Brother;
	}
}

bool MMParts_Root::Cancel()
{
	plaSize->SetBaseValue(0);
	auto itr = (MMParts_Draw*)Child;
	FOR(ChildNum)
	{
		itr->MMParts_Draw::Cancel();
		itr = (MMParts_Draw*)itr->Brother;
	}
	return true;
}

void MMParts_Root::AllCancel()
{
	//子を閉じてから自分を閉じ
	auto itr = (MMParts_Draw*)Child;
	FOR(ChildNum)
	{
		itr->AllCancel();
		itr = (MMParts_Draw*)itr->Brother;
	}
	Cancel();
}

void MMParts_Root::Move()
{
	MMParts_Draw::Move();

	if (plaSize->GetValue() <= 0.01)return; //メニューが閉じているので抜ける

	auto itr = (MMParts_Draw*)Child;
	FOR(ChildNum)
	{
		itr->Move();
		itr = (MMParts_Draw*)itr->Brother;
	}
}

void MMParts_Root::Draw()
{
	MMParts_Draw::Draw();

	if (plaSize->GetValue() <= 0.001)return; //メニューが閉じているので抜ける
	
	//以下 子メニュー表示中
	auto itr = Child;
	FOR(ChildNum)
	{
		itr->Draw();
		itr = itr->Brother;
	}
}

//////////////////////////////////////////////////////
// Folder

void MMParts_Folder::Appear()
{
	MMParts_Draw::Appear();
	////SetChildPos
	//auto itr = (MMParts_Draw*)Child;
	//FOR(ChildNum)
	//{
	//	itr->MMParts_Draw::Appear();
	//	itr->SetPos(x_texel + x_Dir, y_texel + y_Dir, 0.3*sin(twoPHI*i / (float)ChildNum)*AspectDistortion, 0.3*cos(twoPHI*i / (float)ChildNum), AspectDistortion*0.95);
	//	itr = (MMParts_Draw*)itr->Brother;
	//}
}

void MMParts_Folder::Select()
{
	Active();
	//SetChildPos
	auto itr = (MMParts_Draw*)Child;
	FOR(ChildNum)
	{
		itr->Active();
		itr->/*MMParts_Draw::*/Appear();
		itr->SetPos(x_texel + x_Dir, y_texel + y_Dir, 0.3*sin(twoPHI*i / (float)ChildNum)*AspectDistortion, 0.3*cos(twoPHI*i / (float)ChildNum), AspectDistortion*0.95);
		itr = (MMParts_Draw*)itr->Brother;
	}
}

bool MMParts_Folder::Cancel()
{
	//子メニューを閉じる
	auto itr = (MMParts_Draw*)Child;
	FOR(ChildNum)
	{
		itr->MMParts_Draw::Cancel();
		itr = (MMParts_Draw*)itr->Brother;
	}

	return false;
}

//////////////////////////////////////////////////////
// Leaf

//void MMParts_Leaf::Appear()
//{
//	MMParts_Draw::Appear();
//	//SetChildPos
//	//auto itr = (MMParts_Draw*)Child;
//	//FOR(ChildNum)
//	//{
//	//	itr->MMParts_Draw::Appear();
//	//	itr->SetPos(x_texel, y_texel, 0, 0, AspectDistortion);
//	//	itr = (MMParts_Draw*)itr->Brother;
//	//}
//}