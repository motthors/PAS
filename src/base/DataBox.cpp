#include"DataBox.h"
#include"DDSTextureLoader.h"


DataBox::DataBox()
{
	m_pDevice = 0;
	m_pContext = 0;
	meshIDcounter	= 0;
	texIDcounter	= 0;
	pCreateTexture= 0;
	pCamera		= 0;

	meshTimerID	= -1;
	BillBoardID	= -1;
	QuoteTexID	= -1;
	DotTexID	= -1;
}

DataBox::~DataBox()
{

	int i;
	for(i=0;i<(int)MeshBox.size();++i){
		SAFE_DELETEARRAY(MeshBox[i]->pMat);
		//RELEASE(MeshBox[i]->pMesh);
		SAFE_DELETE(MeshBox[i]);
	}
	for(i=0;i<(int)TextureBox.size();++i){
		RELEASE(TextureBox[i]->pTex);
		RELEASE(TextureBox[i]->pView);
		SAFE_DELETE(TextureBox[i]);
	}
	MeshBox.clear();
	TextureBox.clear();
	SAFE_DELETE(pCreateTexture);
}

void DataBox::Init()
{
	pCreateTexture = new CreateTexture;
	pCreateTexture->SetD3Ddev(m_pDevice, m_pContext);
	pCreateTexture->Init();


	//meshTimerID = GetMesh(_T("data/time.x"));
	//BillBoardID = GetMesh(_T("data/board.x"));
	FOR(99)
	{
		TCHAR text[10];
		_stprintf_s(text,10,_T("%d"),i);
		GetTextureFromString(text,NULL);
	}
	QuoteTexID = GetTextureFromString(_T("'"),NULL);
	DotTexID = GetTextureFromString(_T("."), NULL);
	return;
}

//UINT DataBox::GetData(TCHAR* FileName,TCHAR* FolderName)
//{
//	ID3DXBuffer *pMatBuf;
//	D3DCOLORVALUE MAmbient = {0.2f, 0.2f, 0.2f, 1.0f};
//	TCHAR TempText[100]={0};
//
//
//	//std::vector<MeshClass>::iterator it;
//	unsigned int i;
//	for(i=0;i<(int)MeshBox.size();++i){
//
//		//�t�@�C�����������������炻��ID��Ԃ�
//		_stprintf_s(TempText,100,_T("%s%s\0"),FolderName,FileName);
//		if(wcscmp( MeshBox[i]->filename, TempText)==0){
//			return i;
//		}
//	}
//	//�����t�@�C�������Ȃ�������V���Ƀ��[�h
//	MeshClass *NewM = new MeshClass;
//
//	_stprintf_s(NewM->filename,100,_T("%s%s\0"),FolderName,FileName);
//	if(FAILED(D3DXLoadMeshFromX(NewM->filename , D3DXMESH_MANAGED, pd3ddev, NULL, &pMatBuf, NULL, &NewM->NumMaterials, &NewM->pMesh))){
//		ZeroMemory(TempText,sizeof(TCHAR)*100);
//		_stprintf_s(TempText,100,_T("%s:\n%s\0"),_T("���̃��b�V���t�@�C���ǂݍ��݂Ɏ��s"),FileName);
//		MessageBox(NULL,TempText,_T("xfile load error"),MB_OK);
//		SAFE_DELETE(NewM);
//		return -1;
//	}
//	// ���b�V�������m��
//	NewM->pMat = new D3DMATERIAL9[ NewM->NumMaterials ];
//	// �e�N�X�`��ID�ϐ��𖇐����m��
//	NewM->TextureID = new UINT[NewM->NumMaterials];
//
//	D3DXMATERIAL* d3Mat = (D3DXMATERIAL*)pMatBuf->GetBufferPointer();
//	d3Mat->MatD3D.Ambient = MAmbient; //�A���r�G���g�ݒ�
//	
//	for(i=0;i<NewM->NumMaterials;++i){
//		//�}�e���A�������o��
//		NewM->pMat[i] = d3Mat[i].MatD3D;					// �}�e���A�����Z�b�g
//		NewM->pMat[i].Ambient	= NewM->pMat[i].Diffuse;	// ����������
//
//		//�e�N�X�`���쐬
//		//�e�N�X�`�����Ȃ�������A�{�[��
//		if(d3Mat[i].pTextureFilename==NULL)continue;
//		if(strcmp(d3Mat[i].pTextureFilename,"")==0)continue;
//
//		size_t origsize = strlen(d3Mat[i].pTextureFilename) + 1;
//		ZeroMemory(TempText,sizeof(TCHAR)*100);
//		mbstowcs_s(NULL, TempText, origsize, d3Mat[i].pTextureFilename, _TRUNCATE);
//		if(FAILED(NewM->TextureID[i]=GetTexture(TempText,FolderName))){
//			SAFE_DELETEARRAY(NewM->pMat);
//			SAFE_DELETEARRAY(NewM->TextureID);
//			SAFE_DELETE(NewM);
//			return -2;
//		};
//
//	}
//	RELEASE(pMatBuf);
//
//	MeshBox.push_back(NewM);
//
//	return meshIDcounter++;
//}

UINT DataBox::GetMesh(TCHAR* URL)
{
	//ID3DXBuffer *pMatBuf;
	D3DCOLORVALUE MAmbient = {0.2f, 0.2f, 0.2f, 1.0f};
	TCHAR TempText[300]={0};

	UINT i;
	for(i=0;i<(int)MeshBox.size();++i)
	{
		//�t�@�C�����������������炻��ID��Ԃ�
		if(wcscmp( MeshBox[i]->filename, URL)==0)
		{
			return i;
		}
	}
	//�����t�@�C�������Ȃ�������V���Ƀ��[�h
	MeshClass *NewM = new MeshClass;

	_stprintf_s(NewM->filename,100,_T("%s\0"),URL);
	//if(FAILED(D3DXLoadMeshFromX(NewM->filename,
	//						D3DXMESH_MANAGED,
	//						pd3ddev,
	//						NULL, 
	//						&pMatBuf,
	//						NULL,
	//						&NewM->NumMaterials,
	//						&NewM->pMesh)
	//		)
	//	)
	//{
	//	//�t�@�C���ǂݍ��ݎ��s
	//	ZeroMemory(TempText,sizeof(TCHAR)*300);
	//	_stprintf_s(TempText,290,_T("%s\n:%s\0")_T("���̃��b�V���t�@�C���ǂݍ��݂Ɏ��s"),URL);
	//	MessageBox(NULL,TempText,_T("xfile load error"),MB_OK);
	//	SAFE_DELETE(NewM);
	//	return -1;
	//}

	////�}�e���A���ݒ�
	//if(pMatBuf)
	//{
	//	NewM->pMat = new D3DMATERIAL9[ NewM->NumMaterials ];
	//	D3DXMATERIAL* d3Mat = (D3DXMATERIAL*)pMatBuf->GetBufferPointer();
	//	d3Mat->MatD3D.Ambient = MAmbient; 
	//
	//	for(i=0;i<NewM->NumMaterials;++i)
	//	{
	//		//�}�e���A�������o��
	//		NewM->pMat[i] = d3Mat[i].MatD3D;					// �}�e���A�����Z�b�g
	//		NewM->pMat[i].Ambient	= NewM->pMat[i].Diffuse;	// ����������

	//	}
	//}
	//RELEASE(pMatBuf);
	MeshBox.push_back(NewM);
	return meshIDcounter++;
}

UINT DataBox::GetTexture(TCHAR* URL)
{
	TCHAR TempText[300]={0};
	HRESULT hr = E_FAIL;

	FOR((int)TextureBox.size())
	{
		//�t�@�C�����������������炻��ID��Ԃ�
		if(wcscmp( TextureBox[i]->filename, URL)==0)
		{
			return i;
		}
	}
	//�����t�@�C�������Ȃ�������V���Ƀ��[�h
	TextureClass *NewT = new TextureClass;

	//�e�N�X�`���쐬
	_stprintf_s(NewT->filename,100,_T("%s\0"),URL);
	ID3D11Resource* resource;
	hr = CreateDDSTextureFromFile(m_pDevice, m_pContext, NewT->filename, &resource, &NewT->pView);
	NewT->pTex = static_cast<ID3D11Texture2D*>(resource);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("DataBox::GetTexture::CreateDDSTextureFromFile"));
		ErrM.SetErrorText(_T("�e�N�X�`���̓ǂݍ��݂Ɏ��s"));
		ErrM.SetHResult(hr);
		SAFE_DELETE(NewT);
		throw &ErrM;
	}

	TextureBox.push_back(NewT);
	return texIDcounter++;
}

UINT DataBox::GetTextureFromString(TCHAR* text, float *pRatio, int fontsize)
{
	TCHAR TempText[100]={0};
	HRESULT hr;

	int i;
	for(i=0;i<(int)TextureBox.size();++i){
		//�t�@�C�����������������炻��ID��Ԃ�
		if(wcscmp( TextureBox[i]->filename, text)==0){
			if(pRatio!=NULL)*pRatio = _tcslen(text);//TextureBox[i]->pTex->
			return i;
		}
	}
	//�����t�@�C�������Ȃ�������V���ɍ쐬
	TextureClass *NewT = new TextureClass;

	//�e�N�X�`���쐬
	_stprintf_s(NewT->filename,100,_T("%s\0"),text);
	
	pCreateTexture->FromString(text, &NewT->pTex,&NewT->pView, pRatio, fontsize);

	TextureBox.push_back(NewT);
	return texIDcounter++;
}

//ElaseMesh(){}
//ElaseTexture(){}

//void DataBox::Draw(UINT ID)
//{
//#ifdef _DEBUG
//	if(ID < 0)return;
//	if(MeshBox[ID]->TextureID[0] == 0)return;
//#endif
//
//	//pd3ddev->SetTransform(D3DTS_WORLD, world);
//
//	for(unsigned int j=0; j<MeshBox[ID]->NumMaterials; ++j){ 
//		pd3ddev->SetMaterial( &MeshBox[ID]->pMat[j] );
//		pd3ddev->SetTexture(0, TextureBox[MeshBox[ID]->TextureID[j]]->pTex );
//		MeshBox[ID]->pMesh->DrawSubset(j); 
//	}
//}

void DataBox::Draw(UINT MeshID, UINT TextureID)
{
//#ifdef _DEBUG
//	if(MeshID < 0)return;
//	if(TextureID < 0)return;
//#endif
//
//	FOR(MeshBox[MeshID]->NumMaterials)
//	{ 
//		pd3ddev->SetMaterial(&MeshBox[MeshID]->pMat[i]);
//		pd3ddev->SetTexture(0, TextureBox[TextureID]->pTex);
//		//MeshBox[MeshID]->pMesh->DrawSubset(i);
//	}
}


void DataBox::DrawMesh(UINT MeshID)
{
//#ifdef _DEBUG
//	if(MeshID < 0)return;
//#endif
//
//	for(unsigned int j=0; j<MeshBox[MeshID]->NumMaterials; ++j)
//	{ 
//		//m_pDevice->SetMaterial(&MeshBox[MeshID]->pMat[j]);
//		//MeshBox[MeshID]->pMesh->DrawSubset(j); 
//	}
//
}


void DataBox::SetTexture(UINT ID, UINT StartSlot/* =0 */, UINT NumViews/* =1 */)
{
#ifdef _DEBUG
	if (ID < 0){ ALERT(_T("DataBox::SetTexture ... ID���ُ�")); return; }
#endif
	m_pContext->PSSetShaderResources(StartSlot, NumViews, &TextureBox[ID]->pView);
}
//void DataBox::SetTexture(UINT ID, ID3DXEffect* pEffect, char* VarName)
//{
//	pEffect->SetTexture(VarName, TextureBox[ID]->pTex);
//}
//void DataBox::SetTexture(UINT ID, ID3DXEffect* pEffect, D3DXHANDLE Handle)
//{
//	pEffect->SetTexture(Handle, TextureBox[ID]->pTex);
//}

void DataBox::DrawOneMesh(UINT MeshID, int meshnum)
{
#ifdef _DEBUG
	if (MeshID < 0)return;
#endif

	//pd3ddev->SetMaterial(&MeshBox[MeshID]->pMat[meshnum]);
	//MeshBox[MeshID]->pMesh->DrawSubset(meshnum);
}

void DataBox::DrawOneMesh(UINT MeshID, UINT TexID, int meshnum)
{
#ifdef _DEBUG
	if(MeshID < 0)return;
	if(TexID < 0)return;
#endif

	//pd3ddev->SetMaterial( &MeshBox[MeshID]->pMat[meshnum] );
	//pd3ddev->SetTexture(0, TextureBox[TexID]->pTex );
	//MeshBox[MeshID]->pMesh->DrawSubset(meshnum); 
}

void DataBox::DrawBillBoard(UINT TextureID)
{
#ifdef _DEBUG
	if(TextureID < 0)return;
#endif
	//pd3ddev->SetMaterial( MeshBox[BillBoardID]->pMat );
	//pd3ddev->SetTexture(0, TextureBox[TextureID]->pTex );
	//MeshBox[BillBoardID]->pMesh->DrawSubset(0);
}
void DataBox::DrawBillBoard()
{
	//pd3ddev->SetMaterial(MeshBox[BillBoardID]->pMat);
	//MeshBox[BillBoardID]->pMesh->DrawSubset(0);
}

void DataBox::DrawTime(DWORD mSec)
{
	// �~���b
	FOR(12)
	{
		DrawOneMesh(meshTimerID,i,i);
	}
}



//////////////////////////////////////////////////////
#ifdef _DEBUG

void DataBox::DebugerInit()
{
	debugerMeshID = GetMesh(_T("data/board.x"));
	debugerTexID  = GetTexture(_T("data/particle/debuger.png"));
}

void DataBox::Debuger(vec *pos, float scale)
{
	//ChangeRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	mat world;
	world = XMMatrixIdentity();
	world = XMMatrixScaling(0.5f*scale, 0.5f*scale, 0.5f*scale);
	//M_MatrixOffset(&world, pos);
	Draw(debugerMeshID,debugerTexID);
	//ChangeRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

#endif