#include"MenuManager.h"
#include"../base/DefineRendering.h"
#include"MMParts_Draw.h"
#include"MMParts_SlideBar.h"
#include"MMParts_Func.h"

MenuManager::MenuManager(virtualController* pvc, DataBox* pdb, My2DDrawManager* p2d)
{
	idx = 1;
	pInput = pvc;
	pDataBox = pdb;
	p2DDrawer = p2d;
	AspectDistortion = (float)DefRender.WindowY / (float)DefRender.WindowX;
	MouseOnMenuFlag = false;
}

void MenuManager::AddMenuStruct_Root(TCHAR* name)
{
	MMParts_Root* MMProot = new MMParts_Root;
	MMProot->Set2DDrawer(p2DDrawer);
	MMProot->size = 0.08f;
	MMProot->texidName = pDataBox->GetTextureFromString(name, &MMProot->texwidthName, 20);
	AddMenuStruct(0, MMProot, name);
}

void MenuManager::AddMenuStruct_Folder(int Layer, TCHAR* name)
{
	MMParts_Folder* MMP = new MMParts_Folder;
	MMP->Set2DDrawer(p2DDrawer);
	MMP->size = 0.1f;
	MMP->texidName = pDataBox->GetTextureFromString(name, &MMP->texwidthName, 20);
	AddMenuStruct(Layer, MMP, name);
}

void MenuManager::AddMenuStruct_Leaf(int Layer, TCHAR* name)
{
	MMParts_Leaf* MMP = new MMParts_Leaf;
	MMP->Set2DDrawer(p2DDrawer);
	MMP->size = 0.08f;
	MMP->texidName = pDataBox->GetTextureFromString(name, &MMP->texwidthName, 20);
	AddMenuStruct(Layer, MMP, name);
}

void MenuManager::AddMenuStruct_Func(int Layer, TCHAR* name, std::function<void()> func)
{
	MMParts_Func* MMP = new MMParts_Func(func);
	MMP->Set2DDrawer(p2DDrawer);
	MMP->size = 0.08f;
	MMP->texidName = pDataBox->GetTextureFromString(name, &MMP->texwidthName, 20);
	AddMenuStruct(Layer, MMP, name);
}

inline MMParts_SlideBar* MMAMSS1(int IntVariableflag, float* pVariable, virtualController* pInput, My2DDrawManager* p2DDrawer)
{
	MMParts_SlideBar* MMP;
	if (IntVariableflag)MMP = new MMParts_SlideBarI;
	else MMP = new MMParts_SlideBar;
	MMP->SetVariable(pVariable);
	MMP->SetMouseInput(pInput->Debug_GetInputPointer());
	MMP->Set2DDrawer(p2DDrawer);
	MMP->SetBaseSize(0.08f);
	return MMP;
}
inline void MMAMSS2(MenuManager* This, MMParts_SlideBar* MMP, int LeafLayer, DataBox* pDataBox, TCHAR* name, int Layer)
{
	MMP->size = 0.09f + LeafLayer*0.09f;
	MMP->texidName = pDataBox->GetTextureFromString(name, &MMP->texwidthName, 20);
	This->AddMenuStruct(Layer, MMP, name);
}
void MenuManager::AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float* pVariable,
	float MinVar, float MaxVar, int IntVariableflag)
{
	MMParts_SlideBar* MMP = MMAMSS1(IntVariableflag, pVariable, pInput, p2DDrawer);
	MMP->SetMin(MinVar);
	MMP->SetMax(MaxVar);
	MMAMSS2(this, MMP, LeafLayer, pDataBox, name, Layer);
}
void MenuManager::AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float* pVariable,
	float* pMin, float MaxVar, int IntVariableflag)
{
	MMParts_SlideBar* MMP = MMAMSS1(IntVariableflag, pVariable, pInput, p2DDrawer);
	MMP->SetMin_p(pMin);
	MMP->SetMax(MaxVar);
	MMAMSS2(this, MMP, LeafLayer, pDataBox, name, Layer);
}
void MenuManager::AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float* pVariable,
	float MinVar, float* pMax, int IntVariableflag)
{
	MMParts_SlideBar* MMP = MMAMSS1(IntVariableflag, pVariable, pInput, p2DDrawer);
	MMP->SetMin(MinVar);
	MMP->SetMax_p(pMax);
	MMAMSS2(this, MMP, LeafLayer, pDataBox, name, Layer);
}
void MenuManager::AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float* pVariable,
	float* pMin, float* pMax, int IntVariableflag)
{
	MMParts_SlideBar* MMP = MMAMSS1(IntVariableflag, pVariable, pInput, p2DDrawer);
	MMP->SetMin_p(pMin);
	MMP->SetMax_p(pMax);
	MMAMSS2(this, MMP, LeafLayer, pDataBox, name, Layer);
}
////////////////////////////////
inline MMParts_SlideBar* MMAMSS1p(int IntVariableflag, float** ppVariable, virtualController* pInput, My2DDrawManager* p2DDrawer)
{
	MMParts_SlideBarPF* MMP;
	/*if (IntVariableflag)*/
	MMP = new MMParts_SlideBarPF;
	//else MMP = new MMParts_SlideBar;
	MMP->SetVariable(ppVariable);
	MMP->SetMouseInput(pInput->Debug_GetInputPointer());
	MMP->Set2DDrawer(p2DDrawer);
	MMP->SetBaseSize(0.08f);
	return MMP;
}
void MenuManager::AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float** ppVariable,
	float MinVar, float MaxVar, int IntVariableflag)
{
	MMParts_SlideBar* MMP = MMAMSS1p(IntVariableflag, ppVariable, pInput, p2DDrawer);
	MMP->SetMin(MinVar);
	MMP->SetMax(MaxVar);
	MMAMSS2(this, MMP, LeafLayer, pDataBox, name, Layer);
}
void MenuManager::AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float** ppVariable,
	float* pMin, float MaxVar, int IntVariableflag)
{
	MMParts_SlideBar* MMP = MMAMSS1p(IntVariableflag, ppVariable, pInput, p2DDrawer);
	MMP->SetMin_p(pMin);
	MMP->SetMax(MaxVar);
	MMAMSS2(this, MMP, LeafLayer, pDataBox, name, Layer);
}
void MenuManager::AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float** ppVariable,
	float MinVar, float* pMax, int IntVariableflag)
{
	MMParts_SlideBar* MMP = MMAMSS1p(IntVariableflag, ppVariable, pInput, p2DDrawer);
	MMP->SetMin(MinVar);
	MMP->SetMax_p(pMax);
	MMAMSS2(this, MMP, LeafLayer, pDataBox, name, Layer);
}
void MenuManager::AddMenuStruct_SlideBar(int Layer, TCHAR* name, int LeafLayer, float** ppVariable,
	float* pMin, float* pMax, int IntVariableflag)
{
	MMParts_SlideBar* MMP = MMAMSS1p(IntVariableflag, ppVariable, pInput, p2DDrawer);
	MMP->SetMin_p(pMin);
	MMP->SetMax_p(pMax);
	MMAMSS2(this, MMP, LeafLayer, pDataBox, name, Layer);
}


void MenuManager::AddMenuStruct_SlideBar_Color(int Layer, TCHAR* name,
								vec3* pVariable, float min, float max)
{
	float* var[3] = { &(pVariable->x), &(pVariable->y), &(pVariable->z) };
	TCHAR* col[3] = { _T(" : R"), _T(" : G"), _T(" : B") };
	FOR(3)
	{
		MMParts_SlideBar* MMP = new MMParts_SlideBar;
		MMP->SetVariable(var[i]);
		MMP->SetMouseInput(pInput->Debug_GetInputPointer());
		MMP->Set2DDrawer(p2DDrawer);
		MMP->SetBaseSize(0.08f);
		MMP->SetMin(min);
		MMP->SetMax(max);
		MMP->size = 0.09f + (i+1)*0.09f;
		TCHAR AddName[30] = { 0 };
		_tcscat_s(AddName, 29, name);
		_tcscat_s(AddName, 29, col[i]);
		MMP->texidName = pDataBox->GetTextureFromString(AddName, &MMP->texwidthName, 20);
		AddMenuStruct(Layer, MMP, AddName);
	}
}

void MenuManager::SetClickedMenu()
//クリックした瞬間に押されたメニューを記録 > クリックが離れた瞬間の処理時に使う
{
	float x, y;
	SetRootMenuPos(&x, &y);
	MMParts_Base* itr = GetIterator();
	BYTE test;
	//親の判定
	switch (test = itr->SelectJudge(x, y, AspectDistortion))
	{
	case 1:	// 通常のメニュー項目がクリックされた
		MouseOnMenuFlag = true;
		mmpClicked = itr;
		return;
	case 2:	// SlideBar専用　つまみがクリックされた
		MouseOnMenuFlag = true;
		return;
	case 3:	// SlideBar専用  つまみはクリックされなかったが、子メニュー判定はしない
		return;
	}

	itr = itr->Child;
	// 子メニューを順番に判定
	while (1)
	{
		// どこかで判定があるならそれを選択 break -> Select()
		switch (test = itr->SelectJudge(x, y, AspectDistortion))
		{
		case 1:	// 通常の選択がされた
			MouseOnMenuFlag = true;
			goto BREAK;
		case 2:	// SlideBar専用　つまみがクリックされた
			MouseOnMenuFlag = true;
			return;
		case 3:	// SlideBar専用　つまみはクリックされなかったが、子メニュー判定はしない
			break;	// 何もしない
		}
		// 次の兄弟を見に行って、無かったら終了
		if ((itr = itr->Brother) == nullptr)
		{
			mmpClicked = nullptr;
			MouseOnMenuFlag = false;
			return;	// どこにも判定がなかった
		}
	}
	
BREAK:
	mmpClicked = itr;
}

void MenuManager::ClickDecision()
// クリックが切れた瞬間に呼ばれる
// アクティブなメニュー項目のどれかがクリックされたかどうか判定
{
	float x, y;
	SetRootMenuPos(&x, &y);
	auto itr = GetIterator();
	// 親の判定
	if (itr->SelectJudge(x, y, AspectDistortion))
	{
		if (itr == mmpClicked) // クリックした瞬間と同じならOK
		{
			if (itr->Cancel())
			{
				TakeInAndOutMenu(); // メニュー終了ならフラグ倒し
				return;
			}
			MoveIterator_Cancel();
			if (itr->Parent)itr->Parent->Active();
			return;
		}
	}
	
	MoveIterator_Select();
	// 子メニューを順番に判定
	while (1)
	{
		if (GetIterator() == mmpClicked) // クリックした瞬間と同じならループを抜けてSelect実行
		{
			// どこかで判定があるならそれを選択 break -> Select()
			if (GetIterator()->SelectJudge(x, y, AspectDistortion))break;
			// 次の兄弟を見に行って、無かったら終了
		}
		if (MoveIterator_Down() == false)
		{
			MoveIterator_Cancel();
			MouseOnMenuFlag = false;
			return;	// どこにも判定がなかった
		}
	}
	itr->Inactive();			// 自分の非アクティブ
	GetIterator()->Select();	// 子のアクティブ
}

void MenuManager::ShiftMenuStructs()
{
	auto pinpt = pInput->Debug_GetInputPointer();
	LONG moveX;
	LONG moveY;
	if (mmpMenuDrag == nullptr)
	{		
		DragX += moveX = pinpt->GetMouse_X_Relative();
		DragY += moveY = pinpt->GetMouse_Y_Relative();
		if (5 <= abs(DragX) || 5 <= abs(DragY))
		{
			pinpt->GetMouse_Absolute(&DragX, &DragY);
			mmpMenuDrag = mmpClicked;
			mmpClicked = nullptr;
			//MouseOnMenuFlag = (bool)mmpMenuDrag;
		}
	}
	else
	{
		// メニューをドラッグ中
		// ピクセル座標をテクセル座標へ
		float xFix, yFix;
		pinpt->GetMouse_Absolute(&moveX, &moveY);
		DefRender.ConvertToTexelBaseCoord_Relative(&xFix, &yFix, moveX - DragX, moveY - DragY);
		GetMenuArr()->Drag(xFix, yFix);
		DragX = moveX;
		DragY = moveY;
		MouseOnMenuFlag = true;
	}
}

void MenuManager::TakeInAndOutMenu()
{
	InOutFlag = !InOutFlag;
	if (InOutFlag)//メニューが開いたら
	{
		MoveIterator_Reset();
		float x, y;
		SetRootMenuPos(&x, &y);
		auto itr = GetIterator();
		itr->SetPos(x, y, 0, 0, AspectDistortion);
		itr->Appear();
	}
	else //メニューが閉じたら
	{
		MoveIterator_Reset();
		GetIterator()->AllCancel();
		MouseOnMenuFlag = false;
	}
}

void MenuManager::SetRootMenuPos(float* xFix, float* yFix)
{
	LONG x, y;
	pInput->Debug_GetInputPointer()->GetMouse_Absolute(&x, &y);
	DefRender.ConvertToTexelBaseCoord(xFix, yFix, x, y);
}

bool MenuManager::CheckOnMouse()
{
	return MouseOnMenuFlag;
}

void MenuManager::Move()
{
	//メニュー出し入れ
	if (pInput->Debug_GetInputPointer()->GetMouseButtonOn(INPUT_MOUSE_CENTER))
	{
		MouseOnMenuFlag = false;
		TakeInAndOutMenu();
	}

	auto itr = GetMenuArr();
	itr->Move();

	//メニューが開いていなかったら以下の処理は受け付けない
	if (InOutFlag == 0)return;


	//コントローラ操作用処理//////////////////		 //
	if (pInput->Up())								 //
	{												 //
		idx--;										 //
		if (idx < 1)idx = 1;						 //
	}												 //
	if (pInput->Down())								 //
	{												 //
		idx++;										 //
		if (idx > GetChildNum())idx = GetChildNum(); //
	}												 //
	if (pInput->OK())								 //
	{												 //
		MoveIterator_Select();						 //
		FOR(idx - 1)MoveIterator_Down();			 //
	}												 //
	if (pInput->Cancel())							 //
	{												 //
		MoveIterator_Cancel();						 //
	}												 //
	//コントローラ操作用処理ここまで////////////	 //


	///////// マウス操作用処理///////////
	// クリック瞬間
	if (pInput->Debug_GetInputPointer()->GetMouseButtonOn(INPUT_MOUSE_LEFT))
	{
		SetClickedMenu();
	}
	// クリック中
	if (pInput->Debug_GetInputPointer()->GetMouseButton(INPUT_MOUSE_LEFT))
	{
		ShiftMenuStructs();
	}
	// 離した瞬間
	if (pInput->Debug_GetInputPointer()->GetMouseButtonOff(INPUT_MOUSE_LEFT))
	{
		ClickDecision();
		mmpClicked = nullptr;
		mmpMenuDrag = nullptr;
		DragY = DragX = 0;
	}
}

void MenuManager::Draw()
{
	//以下メニュー描画
	auto itr = GetMenuArr();
	itr->Draw();
}

void MenuManager::Draw(DebugCommentManager* p)
{
	//出現フラグが立っていなかったら終わり
	if (!InOutFlag)return;

	//以下メニュー描画
	auto itr = GetIterator();
	p->Set(itr->Name); p->Set(_T("\n"));
	int num = itr->ChildNum;
	itr = (MMParts_Draw*)itr->Child;
	FOR(num)
	{
		p->Set(_T(" :"));
		p->Set(itr->Name);
		if ((i + 1) == idx)p->Set(_T(" <- "));
		p->Set(_T("\n"));
		itr = (MMParts_Draw*)itr->Brother;
		if (itr == nullptr)break;
	}
	p->End();
}
