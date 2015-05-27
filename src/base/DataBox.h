#ifndef _DATA_BOX_H_
#define _DATA_BOX_H_

#include<vector>
#include"IncludeAll.h"
#include"CreateTexture.h"
#include"CameraController.h"

class DataBox{

	DXDEVICE*	m_pDevice;
	DXCONTEXT*	m_pContext;
	//ID3DXEffect			*pEffect;

	CreateTexture* pCreateTexture;

	CameraController* pCamera;

	typedef struct __MESHC{
		DWORD		NumMaterials;
		void/*ID3DXMesh*/	*	pMesh;
		D3DMATERIAL9*	pMat;
		TCHAR		filename[100];
	} MeshClass;

	typedef struct __TEXTUREC{
		DXTEXTURE2D* pTex;
		ID3D11ShaderResourceView* pView;
		WORD		Flag;
		TCHAR	filename[100];
	} TextureClass;

	std::vector<MeshClass*>	MeshBox;
	std::vector<TextureClass*>	TextureBox;

	//汎用メッシュとかテクスチャとか
	int meshIDcounter;
	int texIDcounter;
	int meshTimerID;
	int BillBoardID;
	int QuoteTexID;
	int DotTexID;

public:
	DataBox();
	~DataBox();

	void Init();

	void SetD3Device(DXDEVICE* ddev, DXCONTEXT* pcont){ m_pDevice = ddev; m_pContext = pcont; }
	void SetCamera(CameraController *pcam){pCamera=pcam;}
	//void SetEffect(ID3DXEffect *pEff){pEffect=pEff;}

	//返り値はこのクラスで管理するID　失敗したら負の値
	//UINT GetData(TCHAR* FileName,TCHAR* FolderName); //廃止
	UINT GetMesh(TCHAR* FileName);
	UINT GetTexture(TCHAR* FileName);
	UINT GetTextureFromString(TCHAR* text, float *pRatio, int fontsize=200);

	//void Draw(UINT MeshID);
	void Draw(UINT MeshID, UINT TextureID);
	void DrawMesh(UINT ID);
	void SetTexture(UINT ID, UINT StartSlot = 0, UINT NumViews = 1);
	void DrawOneMesh(UINT MeshID, int num);
	void DrawOneMesh(UINT MeshID, UINT TexID, int num);
	void DrawBillBoard(UINT TextureID);
	void DrawBillBoard();
	void DrawTime(DWORD mSec);

	//特定の記号テクスチャのID　各Draw関数のTextureIDの引数などで使用
	int TexID_Dot(){ return DotTexID; }

	//void ChangeRenderState(D3DRENDERSTATETYPE r, DWORD d){ m_pDevice->SetRenderState(r, d); };


#ifdef _DEBUG
	int debugerMeshID;
	int debugerTexID;
	void DebugerInit();
	void Debuger(vec *pos,float scale=1.0f);
#endif

};

#endif