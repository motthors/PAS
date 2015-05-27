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
//�N���b�N�����u�Ԃɉ����ꂽ���j���[���L�^ > �N���b�N�����ꂽ�u�Ԃ̏������Ɏg��
{
	float x, y;
	SetRootMenuPos(&x, &y);
	MMParts_Base* itr = GetIterator();
	BYTE test;
	//�e�̔���
	switch (test = itr->SelectJudge(x, y, AspectDistortion))
	{
	case 1:	// �ʏ�̃��j���[���ڂ��N���b�N���ꂽ
		MouseOnMenuFlag = true;
		mmpClicked = itr;
		return;
	case 2:	// SlideBar��p�@�܂݂��N���b�N���ꂽ
		MouseOnMenuFlag = true;
		return;
	case 3:	// SlideBar��p  �܂݂̓N���b�N����Ȃ��������A�q���j���[����͂��Ȃ�
		return;
	}

	itr = itr->Child;
	// �q���j���[�����Ԃɔ���
	while (1)
	{
		// �ǂ����Ŕ��肪����Ȃ炻���I�� break -> Select()
		switch (test = itr->SelectJudge(x, y, AspectDistortion))
		{
		case 1:	// �ʏ�̑I�������ꂽ
			MouseOnMenuFlag = true;
			goto BREAK;
		case 2:	// SlideBar��p�@�܂݂��N���b�N���ꂽ
			MouseOnMenuFlag = true;
			return;
		case 3:	// SlideBar��p�@�܂݂̓N���b�N����Ȃ��������A�q���j���[����͂��Ȃ�
			break;	// �������Ȃ�
		}
		// ���̌Z������ɍs���āA����������I��
		if ((itr = itr->Brother) == nullptr)
		{
			mmpClicked = nullptr;
			MouseOnMenuFlag = false;
			return;	// �ǂ��ɂ����肪�Ȃ�����
		}
	}
	
BREAK:
	mmpClicked = itr;
}

void MenuManager::ClickDecision()
// �N���b�N���؂ꂽ�u�ԂɌĂ΂��
// �A�N�e�B�u�ȃ��j���[���ڂ̂ǂꂩ���N���b�N���ꂽ���ǂ�������
{
	float x, y;
	SetRootMenuPos(&x, &y);
	auto itr = GetIterator();
	// �e�̔���
	if (itr->SelectJudge(x, y, AspectDistortion))
	{
		if (itr == mmpClicked) // �N���b�N�����u�ԂƓ����Ȃ�OK
		{
			if (itr->Cancel())
			{
				TakeInAndOutMenu(); // ���j���[�I���Ȃ�t���O�|��
				return;
			}
			MoveIterator_Cancel();
			if (itr->Parent)itr->Parent->Active();
			return;
		}
	}
	
	MoveIterator_Select();
	// �q���j���[�����Ԃɔ���
	while (1)
	{
		if (GetIterator() == mmpClicked) // �N���b�N�����u�ԂƓ����Ȃ烋�[�v�𔲂���Select���s
		{
			// �ǂ����Ŕ��肪����Ȃ炻���I�� break -> Select()
			if (GetIterator()->SelectJudge(x, y, AspectDistortion))break;
			// ���̌Z������ɍs���āA����������I��
		}
		if (MoveIterator_Down() == false)
		{
			MoveIterator_Cancel();
			MouseOnMenuFlag = false;
			return;	// �ǂ��ɂ����肪�Ȃ�����
		}
	}
	itr->Inactive();			// �����̔�A�N�e�B�u
	GetIterator()->Select();	// �q�̃A�N�e�B�u
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
		// ���j���[���h���b�O��
		// �s�N�Z�����W���e�N�Z�����W��
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
	if (InOutFlag)//���j���[���J������
	{
		MoveIterator_Reset();
		float x, y;
		SetRootMenuPos(&x, &y);
		auto itr = GetIterator();
		itr->SetPos(x, y, 0, 0, AspectDistortion);
		itr->Appear();
	}
	else //���j���[��������
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
	//���j���[�o������
	if (pInput->Debug_GetInputPointer()->GetMouseButtonOn(INPUT_MOUSE_CENTER))
	{
		MouseOnMenuFlag = false;
		TakeInAndOutMenu();
	}

	auto itr = GetMenuArr();
	itr->Move();

	//���j���[���J���Ă��Ȃ�������ȉ��̏����͎󂯕t���Ȃ�
	if (InOutFlag == 0)return;


	//�R���g���[������p����//////////////////		 //
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
	//�R���g���[������p���������܂�////////////	 //


	///////// �}�E�X����p����///////////
	// �N���b�N�u��
	if (pInput->Debug_GetInputPointer()->GetMouseButtonOn(INPUT_MOUSE_LEFT))
	{
		SetClickedMenu();
	}
	// �N���b�N��
	if (pInput->Debug_GetInputPointer()->GetMouseButton(INPUT_MOUSE_LEFT))
	{
		ShiftMenuStructs();
	}
	// �������u��
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
	//�ȉ����j���[�`��
	auto itr = GetMenuArr();
	itr->Draw();
}

void MenuManager::Draw(DebugCommentManager* p)
{
	//�o���t���O�������Ă��Ȃ�������I���
	if (!InOutFlag)return;

	//�ȉ����j���[�`��
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
