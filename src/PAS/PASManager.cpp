#include"PASManager.h"

PASManager::PASManager()
{
	MenuM = nullptr;
	m_pEarthView = nullptr;
}


PASManager::~PASManager()
{
	SAFE_DELETE(MenuM);
	SAFE_DELETE(m_pEarthView);
}


int PASManager::Init()
{
	MenuM = new MenuManager(pInput,pDataBox,p2DDrawer);
	m_pEarthView = new EarthViewController(pcam);
	MenuSetup();
	Camera* pc = pcam->GetCameraPointer();
	pc->SetMenuMode();
	pc->GetPosIndex()->x = 0;
	pc->GetPosIndex()->y = 0;
	pc->GetPosIndex()->z = 0;
	BasePoint.x = 0;
	BasePoint.y = pPASCT->GetRg() + 0.1f;
	BasePoint.z = 0;
	Direc.x = 0;
	Direc.y = 0;
	Direc.z = 1.f;
	UP.x = 0;
	UP.y = 1.f;
	UP.z = 0;
	length = 0;

	m_pSunVec = pPASCT->GetpSunDir();
	return 1;
}


void PASManager::MenuSetup()
//root�ł̊e���W���[����Init���I����Ă���ĂԂ���
{
	MenuM->AddMenuStruct_Root(_T("���ǂ�"));

	MenuM->AddMenuStruct_Folder(1, _T("�f��"));
	{
		MenuM->AddMenuStruct_Leaf(2, _T("�傫��"));
		{
			MenuM->AddMenuStruct_SlideBar(3, _T("�n�\���a"), 1, pPASCT->GetpRg(), 100.f, 9000.f, false);
			MenuM->AddMenuStruct_SlideBar(3, _T("��C�����a"), 2, pPASCT->GetpRt(), pPASCT->GetpRg(), 10000.f, false);
			MenuM->AddMenuStruct_SlideBar(3, _T("ڲذ������"), 3, pPASCT->GetpHR(), 0.f, 5.f, false);
			MenuM->AddMenuStruct_SlideBar(3, _T("а������"), 4, pPASCT->GetpHM(), 0.f, 5.f, false);
		}
		MenuM->AddMenuStruct_Folder(2, _T("�U�����ݒ�"));
		{
			MenuM->AddMenuStruct_Folder(3, _T("���C���[�U��"));
			{
				MenuM->AddMenuStruct_Leaf(4, _T("������"));
				{
					MenuM->AddMenuStruct_SlideBar(5, _T("�J���[1"), 1, pPASCT->GetppBetaR(0), 0.f, 5.f, false);
					MenuM->AddMenuStruct_SlideBar(5, _T("�J���[2"), 2, pPASCT->GetppBetaR(1), 0.f, 5.f, false);
					MenuM->AddMenuStruct_SlideBar(5, _T("�J���[3"), 3, pPASCT->GetppBetaR(2), 0.f, 5.f, false);
					//MenuM->AddMenuStruct_SlideBar_Color(4, _T("ڲذ"), pPASCT->GetpBetaR_vec(), 1.0e-4f, 1.0e-1f);
				}
				MenuM->AddMenuStruct_Folder(4, _T("�J���[�ݒ�"));
				{
					MenuM->AddMenuStruct_Leaf(5, _T("�J���[1"));
					{
						MenuM->AddMenuStruct_SlideBar(6, _T("�J���[1"), 1, pPASCT->GetppColor1(0), 0.f, 1.f, false);
						MenuM->AddMenuStruct_SlideBar(6, _T("�J���[1"), 2, pPASCT->GetppColor1(1), 0.f, 1.f, false);
						MenuM->AddMenuStruct_SlideBar(6, _T("�J���[1"), 3, pPASCT->GetppColor1(2), 0.f, 1.f, false);
					}
					MenuM->AddMenuStruct_Leaf(5, _T("�J���[2"));
					{
						MenuM->AddMenuStruct_SlideBar(6, _T("�J���[2"), 1, pPASCT->GetppColor2(0), 0.f, 1.f, false);
						MenuM->AddMenuStruct_SlideBar(6, _T("�J���[2"), 2, pPASCT->GetppColor2(1), 0.f, 1.f, false);
						MenuM->AddMenuStruct_SlideBar(6, _T("�J���[2"), 3, pPASCT->GetppColor2(2), 0.f, 1.f, false);
					}
					MenuM->AddMenuStruct_Leaf(5, _T("�J���[3"));
					{
						MenuM->AddMenuStruct_SlideBar(6, _T("�J���[3"), 1, pPASCT->GetppColor3(0), 0.f, 1.f, false);
						MenuM->AddMenuStruct_SlideBar(6, _T("�J���[3"), 2, pPASCT->GetppColor3(1), 0.f, 1.f, false);
						MenuM->AddMenuStruct_SlideBar(6, _T("�J���[3"), 3, pPASCT->GetppColor3(2), 0.f, 1.f, false);
					}
				}
			}
			MenuM->AddMenuStruct_Leaf(3, _T("�~�[�U��"));
			{
				MenuM->AddMenuStruct_SlideBar(4, _T("�U���x��"), 1, pPASCT->GetppBetaMSca(0), 0.f, 5.f, false);
				//MenuM->AddMenuStruct_SlideBar(4, _T("�J���[2"), 2, pPASCT->GetppBetaR(1), 0.f, 5.f, false);
				//MenuM->AddMenuStruct_SlideBar(4, _T("�J���[3"), 3, pPASCT->GetppBetaR(2), 0.f, 5.f, false);
				//MenuM->AddMenuStruct_SlideBar_Color(4, _T("а"), pPASCT->GetpbetaMSca_vec(), 1.0e-4f, 1.0e-1f);
				//MenuM->AddMenuStruct_SlideBar(4, _T("а�U������"), 4, pPASCT->GetpMieG(), 0.f, 2.0f, false);
			}
		}
	}
	MenuM->AddMenuStruct_Folder(1, _T("�P��"));
	{
		MenuM->AddMenuStruct_Leaf(2, _T("�傫��"));
			MenuM->AddMenuStruct_SlideBar(3, _T(" "), 1, &length, 0.f, 20000.0f, false);
		MenuM->AddMenuStruct_Leaf(2, _T("����"));
			MenuM->AddMenuStruct_SlideBar(3, _T(" "), 1, &length, 0.f, 2.0f, false);
		MenuM->AddMenuStruct_Leaf(2, _T("�F"));
			MenuM->AddMenuStruct_SlideBar_Color(3, _T("�F"), pPASCT->GetpbetaMSca_vec(), 0.f, 1.0f);
	}
	MenuM->AddMenuStruct_Folder(1, _T("�F��"));
	{
		MenuM->AddMenuStruct_Folder(2, _T("�F"));
	}
	MenuM->AddMenuStruct_Leaf(1, _T("�J��������"));
		MenuM->AddMenuStruct_SlideBar(2, _T(" "), 1, &m_pEarthView->m_LengthCDir, 0.f, 20000.0f, false);

	auto f1 = bind(&PASComputeTexture::ResetVariable, pPASCT);
	MenuM->AddMenuStruct_Func(1, _T("���Z�b�g"), f1);
	
	f1 = bind(&PASComputeTexture::StartPrerender, pPASCT);
	MenuM->AddMenuStruct_Func(1, _T("�Čv�Z"), f1);

	MenuM->EndSetUp();
}


void PASManager::Reset()
{
	pcam->GetCameraPointer()->Reset();
}


void PASManager::End()
{
	Reset();
}


void NewTexture2D(vec4 **Arr, int x, int y){
	Arr = new vec4*[x];
	FOR(x)	Arr[i] = new vec4[y];
}
void NewTexture3D(vec4 ***Arr, int x, int y, int z){
	Arr = new vec4**[x];
	FOR(x){
		Arr[i] = new vec4*[y];
		FOR2(y) Arr[i][j] = new vec4[z];
	}
}


void PASManager::InputControll_Mouse_Key()
// �J�������_�ύX �}�E�X�{�L�[�{�[�h�p
{
	Input* pIp = pInput->Debug_GetInputPointer();

	// ���O�v�Z�J�n
	if (pIp->GetKeyNow(DIK_S)) pPASCT->StartPrerender();
	// ���O�v�Z �}���`�U���v�Z�ǉ�
	if (pIp->GetKeyNow(DIK_A)) pPASCT->AddMultiScatter();
	// PAS�e�N�X�`���ۑ�
	if (pIp->GetKeyNow(DIK_SPACE)) pPASCT->SaveTexture();

	float x, y;
	// R�X�e�B�b�N�ʏ�Ŏ��_�ύX shift+R�㉺�ŃJ�������x�ύX
	float local_value;
	local_value = -pInput->RY();
	if (abs(local_value) > 0.1f){
		if (pIp->GetButton(_VC_L2, 0))
			m_pEarthView->MoveAngle(local_value*0.05f);
		else
			pcam->GetCameraPointer()->SetCameraRotaY(local_value*0.06f, 1);
	}
	local_value = pInput->RX();
	if (abs(local_value) > 0.1f){
		m_pEarthView->MoveDirection(local_value*0.04f);
	}

	// ���N���b�N�Ŏ��_�ړ��@Shift+���N���b�N�ňʒu�ړ�
	if (pIp->GetMouseButton(INPUT_MOUSE_LEFT))
	{
		if(pIp->GetKey(DIK_LSHIFT))
		{
			y = 0.0003f*(float)pIp->GetMouse_Y_Relative();
			x = 0.0003f*(float)pIp->GetMouse_X_Relative();
			m_pEarthView->MoveGround(x, y);
		}
		else
		{
			y = (float)pIp->GetMouse_Y_Relative();
			x = (float)pIp->GetMouse_X_Relative();
			pcam->GetCameraPointer()->SetCameraRotaY(y*0.006f, 1);
			m_pEarthView->MoveDirection(x*-0.004f);
		}
	}
	// �E�N���b�N�h���b�O�ő��z�ړ�
	if (pIp->GetMouseButton(INPUT_MOUSE_RIGHT))
	{
		y = 0.002f*(float)pIp->GetMouse_Y_Relative();
		x = -0.002f*(float)pIp->GetMouse_X_Relative();
		TurnSunVec(x, y);
	}
	// �z�C�[����]�ŃJ������������
	m_pEarthView->MoveLength(0.01f*(float)pIp->GetMouse_Wheel());
}


void PASManager::InputControll_Pad()
// �J�������_�ύX �R���g���[���p�b�h�p
{
	// ���O�v�Z�J�n
	if (pInput->ButtonN(_VC_SQUARE)) pPASCT->StartPrerender();
	// ���O�v�Z �}���`�U���v�Z�ǉ�
	if (pInput->ButtonN(_VC_TRIANGLE)) pPASCT->AddMultiScatter();

	float x, y;
	// R�X�e�B�b�N�ʏ�Ŏ��_�ύX shift+R�㉺�ŃJ�������x�ύX
	float local_value;
	local_value = -pInput->RY();
	if (abs(local_value) > 0.1f){
		if (pInput->Debug_GetInputPointer()->GetButton(_VC_L2, 0))
			m_pEarthView->MoveAngle(local_value*0.05f);
		else
			pcam->GetCameraPointer()->SetCameraRotaY(local_value*0.06f, 1);
	}
	local_value = pInput->RX();
	if (abs(local_value) > 0.1f){
		m_pEarthView->MoveDirection(local_value*0.04f);
	}
	// L�X�e�B�b�N�ʏ�ŃJ�����ړ��@shift+L�ő��z�ړ�
	x = pInput->LX();
	y = pInput->LY();
	if (fabs(x) > 0.1f || fabs(y) > 0.1f){
		if (pInput->Debug_GetInputPointer()->GetButton(_VC_L2, 0))
			TurnSunVec(x*-0.01f, y*0.01f);
		else
			m_pEarthView->MoveGround(x*-0.004f, y*-0.004f);
	}

	// L1R1�ŃJ������������
	m_pEarthView->MoveLength(pInput->Yaw());
}


void  PASManager::TurnSunVec(float x, float y)
{
	vec t_vecSun = XMLoadFloat3(m_pSunVec);

	mat matRota;
	Camera* pc = pcam->GetCameraPointer();
	vec dir = XMLoadFloat3(pc->GetCameraDir());
	vec pit = XMLoadFloat3(pc->GetCameraUp());
	pit = XMVector3Cross(pit, dir);
	vec up = XMVector3Cross(pit, dir);
	// up��]
	matRota = XMMatrixRotationAxis(up, x);
	t_vecSun = XMVector3Transform(t_vecSun, matRota);
	// pitch��]
	matRota = XMMatrixRotationAxis(pit, y);
	t_vecSun = XMVector3Transform(t_vecSun, matRota);

	XMStoreFloat3(m_pSunVec, t_vecSun);
}


bool PASManager::Move()
{
	float x, y;


	//// Temp�@Test �v�Z���ꂽ�e�N�X�`���m�F�p
	//if (pInput->ButtonN(_VC_SELECT)
	//	 || pInput->Debug_GetInputPointer()->GetKeyNow(DIK_SPACE)) pPASCT->TextureTestViewFlag = 
	//	((pPASCT->TextureTestViewFlag % 10 + 1) % 6) 
	//	+ pPASCT->TextureTestViewFlag - pPASCT->TextureTestViewFlag % 10;
	//if (pInput->ButtonN(_VC_START)) pPASCT->TextureTestViewFlag =
	//	((pPASCT->TextureTestViewFlag / 10 + 1) % 32) * 10
	//	+ pPASCT->TextureTestViewFlag % 10;

	MenuM->Move();
	// ���j���[���삵�Ă��Ȃ�������A���E����
	if ( ! MenuM->CheckOnMouse() )
	{
		InputControll_Mouse_Key();
		InputControll_Pad();
	}
	else
	{
		int test;
		test = 1;
		pDebugComM->Set(_T("test true"));
		pDebugComM->End();
	}

	// �J��������
	vec t_vecG = m_pEarthView->GetGround(BasePoint.y);
	pcam->GetCameraPointer()->PASCamera(
		t_vecG,
		m_pEarthView->GetCamDir(),
		t_vecG,
		m_pEarthView->GetLength()
		);

	// �U���e�N�X�`������
	pPASCT->Move();
	
	return true;
}


void PASManager::Draw()
{
	pPASCT->Draw();
	MenuM->Draw();
	//MenuM->Draw(pDebugComM);
	//p2DDrawer->DrawSquare_TexelStd(0, 0, 0.2, 0);
	//p2DDrawer->DrawSquare_TexelStd(0.5, -0.5, 0.1, 0);
#ifdef _DEBUG
	//�f�o�b�O�R�����g�Ȃ�
	//pcam->DebugDraw();
	DebugDraw();
#endif
}

#ifdef _DEBUG

void PASManager::DebugDraw()
{
	//TCHAR txt[300];
	//_stprintf_s(txt, 299, _T("MainGameVS\ntime : %d"),
	//	pTimeMeasurer->GetTime()
	//	);
	//pDebugComM->Set(txt);
	//pDebugComM->End();
}

#endif

