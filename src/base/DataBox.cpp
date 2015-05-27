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
//		//ファイル名が同じだったらそのIDを返す
//		_stprintf_s(TempText,100,_T("%s%s\0"),FolderName,FileName);
//		if(wcscmp( MeshBox[i]->filename, TempText)==0){
//			return i;
//		}
//	}
//	//同じファイル名がなかったら新たにロード
//	MeshClass *NewM = new MeshClass;
//
//	_stprintf_s(NewM->filename,100,_T("%s%s\0"),FolderName,FileName);
//	if(FAILED(D3DXLoadMeshFromX(NewM->filename , D3DXMESH_MANAGED, pd3ddev, NULL, &pMatBuf, NULL, &NewM->NumMaterials, &NewM->pMesh))){
//		ZeroMemory(TempText,sizeof(TCHAR)*100);
//		_stprintf_s(TempText,100,_T("%s:\n%s\0"),_T("次のメッシュファイル読み込みに失敗"),FileName);
//		MessageBox(NULL,TempText,_T("xfile load error"),MB_OK);
//		SAFE_DELETE(NewM);
//		return -1;
//	}
//	// メッシュ情報を確保
//	NewM->pMat = new D3DMATERIAL9[ NewM->NumMaterials ];
//	// テクスチャID変数を枚数分確保
//	NewM->TextureID = new UINT[NewM->NumMaterials];
//
//	D3DXMATERIAL* d3Mat = (D3DXMATERIAL*)pMatBuf->GetBufferPointer();
//	d3Mat->MatD3D.Ambient = MAmbient; //アンビエント設定
//	
//	for(i=0;i<NewM->NumMaterials;++i){
//		//マテリアル情報取り出し
//		NewM->pMat[i] = d3Mat[i].MatD3D;					// マテリアル情報セット
//		NewM->pMat[i].Ambient	= NewM->pMat[i].Diffuse;	// 環境光初期化
//
//		//テクスチャ作成
//		//テクスチャがなかったらアボーン
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
		//ファイル名が同じだったらそのIDを返す
		if(wcscmp( MeshBox[i]->filename, URL)==0)
		{
			return i;
		}
	}
	//同じファイル名がなかったら新たにロード
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
	//	//ファイル読み込み失敗
	//	ZeroMemory(TempText,sizeof(TCHAR)*300);
	//	_stprintf_s(TempText,290,_T("%s\n:%s\0")_T("次のメッシュファイル読み込みに失敗"),URL);
	//	MessageBox(NULL,TempText,_T("xfile load error"),MB_OK);
	//	SAFE_DELETE(NewM);
	//	return -1;
	//}

	////マテリアル設定
	//if(pMatBuf)
	//{
	//	NewM->pMat = new D3DMATERIAL9[ NewM->NumMaterials ];
	//	D3DXMATERIAL* d3Mat = (D3DXMATERIAL*)pMatBuf->GetBufferPointer();
	//	d3Mat->MatD3D.Ambient = MAmbient; 
	//
	//	for(i=0;i<NewM->NumMaterials;++i)
	//	{
	//		//マテリアル情報取り出し
	//		NewM->pMat[i] = d3Mat[i].MatD3D;					// マテリアル情報セット
	//		NewM->pMat[i].Ambient	= NewM->pMat[i].Diffuse;	// 環境光初期化

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
		//ファイル名が同じだったらそのIDを返す
		if(wcscmp( TextureBox[i]->filename, URL)==0)
		{
			return i;
		}
	}
	//同じファイル名がなかったら新たにロード
	TextureClass *NewT = new TextureClass;

	//テクスチャ作成
	_stprintf_s(NewT->filename,100,_T("%s\0"),URL);
	ID3D11Resource* resource;
	hr = CreateDDSTextureFromFile(m_pDevice, m_pContext, NewT->filename, &resource, &NewT->pView);
	NewT->pTex = static_cast<ID3D11Texture2D*>(resource);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("DataBox::GetTexture::CreateDDSTextureFromFile"));
		ErrM.SetErrorText(_T("テクスチャの読み込みに失敗"));
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
		//ファイル名が同じだったらそのIDを返す
		if(wcscmp( TextureBox[i]->filename, text)==0){
			if(pRatio!=NULL)*pRatio = _tcslen(text);//TextureBox[i]->pTex->
			return i;
		}
	}
	//同じファイル名がなかったら新たに作成
	TextureClass *NewT = new TextureClass;

	//テクスチャ作成
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
	if (ID < 0){ ALERT(_T("DataBox::SetTexture ... IDが異常")); return; }
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
	// ミリ秒
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