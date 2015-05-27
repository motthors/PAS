#include"2DDrawManager.h"
#include"../base/DefineRendering.h"

My2DDrawManager::My2DDrawManager()
{
	m_pConstantBafferVS = nullptr;
	m_pConstantBafferPS = nullptr;
	m_pDx11 = nullptr;
	m_pDataBox = nullptr;
	m_p2Dsq = nullptr;

	per1TexelX = per1TexelY = 0.0;
	list2DBoardArr.reserve(20);
}


My2DDrawManager::~My2DDrawManager()
{
	for (auto i : list2DBoardArr)
	{
		SAFE_DELETE(i);
	}
	list2DBoardArr.clear();
	RELEASE(m_pConstantBafferVS);
	RELEASE(m_pConstantBafferPS);
	SAFE_DELETE(m_p2Dsq);
}


void My2DDrawManager::Init(DirectX11Base* pdx11, DataBox* pdb, int WindowSizeW, int WindowSizeH)
{
	m_pDx11 = pdx11;
	m_pContext = pdx11->GetContext();
	m_pDataBox = pdb;

	per1TexelX = 1.0f / (float)WindowSizeW;
	per1TexelY = 1.0f / (float)WindowSizeH;



	/////////////// �V�F�[�_�[���쐬���� /////////////

	// ���_�V�F�[�_�[���쐬����
	m_idVertexShader = m_pShaderBox->CreateVertexShader(_T("data/hlsl/2DDrawer_VS.cso"));
	// �s�N�Z���V�F�[�_�[���쐬����
	m_idPixelShader[0] = m_pShaderBox->CreatePixelShader(_T("data/hlsl/PS_RefTex.cso"));
	m_idPixelShader[1] = m_pShaderBox->CreatePixelShader(_T("data/hlsl/2DDrawer_PS_Circle.cso"));
	m_idPixelShader[2] = m_pShaderBox->CreatePixelShader(_T("data/hlsl/2DDrawer_PS_Donut.cso"));
	//m_idPixelShader[0] = m_idPixelShader[1];
	//m_idPixelShader[2] = m_idPixelShader[0];

	m_p2Dsq = new D3D2DSQUARE(pdx11, m_pShaderBox, DefRender.RenderTargetX, DefRender.RenderTargetY);
	m_p2Dsq->Init();
	m_p2Dsq->SetVertexShader(m_idVertexShader);
	m_p2Dsq->SetPixelShader(m_idPixelShader[0]);


	//�萔�o�b�t�@�쐬
	m_pDx11->CreateConstantBuffer(&m_pConstantBafferVS, NULL, sizeof(m2DDWConstVS), D3D11_CPU_ACCESS_WRITE);
	m_pDx11->CreateConstantBuffer(&m_pConstantBafferPS, NULL, sizeof(m2DDWConstPS), D3D11_CPU_ACCESS_WRITE);
}


//void My2DDrawManager::Draw(float lux, float luy, float rdx, float rdy, int texid)
//{
//}


void My2DDrawManager::DrawSquare_TexelStd(float centerX, float centerY, float width, float alpha, int texid)
//width�͉�����ł�
{
	m2DDWStruct* newBoard = new m2DDWStruct;
	newBoard->Width_Shift.x = width;
	newBoard->Width_Shift.y = width * per1TexelY / per1TexelX;
	newBoard->Width_Shift.z = centerX;
	newBoard->Width_Shift.w = centerY;
	newBoard->CenterPos.x = centerX;
	newBoard->CenterPos.y = centerY;
	newBoard->texid = texid;
	newBoard->alpha = alpha;
	list2DBoardArr.push_back(newBoard);
}


void My2DDrawManager::Draw(float centerX, float centerY, float width, float height, float alpha, int texid)
{
	m2DDWStruct* newBoard = new m2DDWStruct;
	newBoard->Width_Shift.x = width;
	newBoard->Width_Shift.y = height * per1TexelY / per1TexelX;
	newBoard->Width_Shift.z = centerX;
	newBoard->Width_Shift.w = centerY;
	newBoard->CenterPos.x = centerX;
	newBoard->CenterPos.y = centerY;
	newBoard->texid = texid;
	newBoard->alpha = alpha;
	list2DBoardArr.push_back(newBoard);
}


void My2DDrawManager::DrawNumber(float centerX, float centerY, float height, float alpha, float Number, int MaxDigit, int Decimal)
//MaxDigit:1������̌����ADecimal:�������̌���
{
#ifdef _DEBUG
	if (MaxDigit < 1)MaxDigit = 1;//throw "2DDrawManager::DrawNumber::MaxDigit is too small";
	if (Decimal < 0)Decimal = 0;//throw "2DDrawManager::DrawNumber::Decimal is too small";
	if (Decimal > 6)Decimal = 6;
#endif
	int DrawNum;
	float width = height * 0.6f;		//����1�̉���
	float xShiftCoord;	//�������狁�߂����ԉE�̐�����X���W�i���΁j

	//�����A����̌���0�݂̂�������A���̕������\�����������炷
	if ((int)ceil(Number) != 0)
	{
		int CutNum = (int)floor(log10(pow(10, MaxDigit-1) / ceil(Number) ));
		if (0 < CutNum) MaxDigit -= CutNum;
	}
	else
	{
		MaxDigit = 1;
	}

	if (Decimal <= 0)
	{
		DrawNum = (int)round(Number);
		if (DrawNum >= (int)pow(10, MaxDigit))
			DrawNum = (int)pow(10, MaxDigit) - 1; //���I�[�o�[�Ȃ�9999...�\��
		xShiftCoord = (float)(MaxDigit - 1) * width;
		goto NoDecimal;	//�����_�ȉ��ƃh�b�g�̕\�����I���Ƃ��܂ŃX�L�b�v
	}
	else
	{
		DrawNum = (int)round(Number * pow(10, Decimal));
		if (DrawNum >= (int)pow(10, MaxDigit+Decimal))
			DrawNum = (int)pow(10, MaxDigit+Decimal) - 1; //���I�[�o�[�Ȃ�9999...�\��
		xShiftCoord = (float)(MaxDigit + Decimal)* width;
	}


	//�����������珇�Ԃɕ`��
	//�����ȉ��������[�v
	FOR(Decimal){
		int texIDNum = DrawNum % 10;
		DrawNum /= 10;
		Draw(centerX + xShiftCoord, centerY, width, height, alpha, texIDNum);
		xShiftCoord -= width*2.f;
	}
	//�h�b�g�`��
	xShiftCoord += width*0.5f;
	Draw(centerX + xShiftCoord, centerY, width*0.5f, height, alpha, m_pDataBox->TexID_Dot());
	xShiftCoord -= width*2.f;

NoDecimal:
	//�����_�㌅�����[�v
	FOR(MaxDigit){
		int texIDNum = DrawNum % 10;
		DrawNum /= 10;
		Draw(centerX + xShiftCoord, centerY, width, height, alpha, texIDNum);
		xShiftCoord -= width*2.f;
		if (DrawNum == 0)break;
	}
}


void My2DDrawManager::Render()
{
	float FakeDepth = 1.f/(float)list2DBoardArr.size();
	UINT DepthIdx = 0;

	// �o�^���ꂽ���������[�v���ĕ`��
	for (auto itr : list2DBoardArr)
	{
		// �萔�o�b�t�@�֏������ނ��߂̍\���̐ݒ�
		memcpy_s(&m_ConstVS.Width_Shift, sizeof(vec4), &itr->Width_Shift, sizeof(vec4));
		m_ConstVS.FakeDepth = DepthIdx++ * FakeDepth;
		memcpy_s(&m_ConstPS.Width_Shift, sizeof(vec4), &itr->Width_Shift, sizeof(vec4));
		m_ConstPS.alpha = itr->alpha;
		m_ConstPS.XperY = m_ConstVS.Width_Shift.x / m_ConstVS.Width_Shift.y;

		// VS�֒萔�o�b�t�@�ݒ�
		m_pDx11->SetConstBuffer(m_pConstantBafferVS, reinterpret_cast<void*>(&m_ConstVS), sizeof(m_ConstVS));
		m_pContext->VSSetConstantBuffers(0, 1, &m_pConstantBafferVS);
		// PS�֒萔�o�b�t�@�ݒ�
		m_pDx11->SetConstBuffer(m_pConstantBafferPS, reinterpret_cast<void*>(&m_ConstPS), sizeof(m_ConstPS));
		m_pContext->PSSetConstantBuffers(0, 1, &m_pConstantBafferPS);
		
		if(itr->texid>=0)
			m_pDataBox->SetTexture(itr->texid);

		int passnum = itr->texid < 0 ? -itr->texid : 0; //�g�\��(id<0)�Ȃ�pass(-id)�A�e�N�X�`���\���Ȃ�pass0
		m_p2Dsq->SetPixelShader(m_idPixelShader[passnum]);
		m_p2Dsq->Render();
	}

	// ���񒆐g�͏���
	for (auto i : list2DBoardArr)SAFE_DELETE(i);
	list2DBoardArr.clear();
}
