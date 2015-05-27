#ifndef _BBBM_H_
#define _BBBM_H_

#include"IncludeAll.h"
#include"DataBox.h"
#include"CameraController.h"
#include<list>

class BBB;

class BillBoardBeamManager {

	///DirectX///
	//頂点宣言
	typedef struct{
		vec		pos;	// 座標
		float	u, v;	// UV座標
	} BBBVertex;
	int texID;
	LPDIRECT3DVERTEXDECLARATION9	pVertDecl;
	/////////////

	BBBVertex* pBBBV;	//メッシュ格納領域へのポインタ

	DXDEVICE*	pDev;
	DataBox*	pDataBox;
	CameraController* pCamera;


	std::list<BBB*> listBBBArr;
	int AllVertexNum;	//頂点データの必要総数　DrawCallを１回で済ますため
	float width;		//ビームの太さ
public:
	
	BillBoardBeamManager();
	virtual ~BillBoardBeamManager();

	int Init(DXDEVICE* pdev, DataBox* pdb, CameraController* pcam);

	BBB* CreateBBB(int VertexNum); //ビーム１本分のマネージャー
	void DeleteBBB(BBB* p);			//ビーム消去

	void SetDrawedVertexNum(int num);
	void MeshSet();	//メッシュの展開計算
	void Draw();		//マネージャ内のすべてのビームの描画
};



/////////////////////////////////////////////////////////////////

typedef struct{
	vec pos;
	vec v;
} BBBStruct;

class BBB{
	BillBoardBeamManager* pParentBBBM;

	int VertexNum;
	BBBStruct* pArrVertex;

public:
	BOOL DrawFlag;
	
	BBB(int VertexNum, BillBoardBeamManager* parent);
	~BBB();

	void SetAllVertex(vec *pos);
	void SetVertexPos(vec *pos);

	int GetVertexNum(){ return VertexNum; }
	BBBStruct* GetBBBstruct(){ return pArrVertex; }

	void DrawSet();	//描画の準備通知関数
};


#endif