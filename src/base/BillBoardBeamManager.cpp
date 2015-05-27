#include"BillBoardBeamManager.h"
#include"LoadFileNameList.h"

#define BBBM BillBoardBeamManager


BBBM::BillBoardBeamManager()
{
	pBBBV = nullptr;
	pDataBox = nullptr;
	width = 0.3f;
}


BBBM::~BillBoardBeamManager()
{
	SAFE_DELETEARRAY(pBBBV);
	for (auto i : listBBBArr)
	{
		SAFE_DELETE(i);
	}
	listBBBArr.clear();
	RELEASE(pVertDecl);
}


int BBBM::Init(DXDEVICE* pdev, DataBox* pdb, CameraController* pcam)
{
	AllVertexNum = 0;
	pCamera = pcam;
	pDev = pdev;

	//テクスチャ読み込み
	pDataBox = pdb;
	texID = pdb->GetTexture(LOAD_T_BEAM);

	// 頂点宣言を作成する
	D3DVERTEXELEMENT9 decl[] = {
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
	};
	//if (FAILED(pdev->CreateVertexDeclaration(decl, &pVertDecl))){
	//	return -1;
	//}
	return 0;
}


BBB* BBBM::CreateBBB(int VertexNum)
{
	BBB* pNewBBB;

	if (VertexNum <= 1)	//頂点数が１以下だとメッシュが作れないのでエラー
	{
		return nullptr;
	}

	pNewBBB = new BBB(VertexNum,this);
	listBBBArr.push_back(pNewBBB);
	return pNewBBB;
}


void BBBM::DeleteBBB(BBB* p)
{
	auto b = listBBBArr.begin();
	for (auto b : listBBBArr)
	{
		if (b == p)
		{
			listBBBArr.remove(p);
		}
	}
}


void BBBM::SetDrawedVertexNum(int num)
{
	AllVertexNum += num + 1;
}


void BBBM::MeshSet()
{
	if (AllVertexNum == 0)return; //BBBが登録されていなかったら描画しなくていい

	//メッシュデータを作成する
	SAFE_DELETEARRAY(pBBBV);
	pBBBV = new BBBVertex[AllVertexNum * 2 - 2];
	int count = 0;
	int BeamNum = listBBBArr.size();
	int* ArrIndex = new int[BeamNum];
	BeamNum = 0;
	auto b = listBBBArr.begin();
	for (; b != listBBBArr.end(); b++)
	{
		if ((*b)->DrawFlag == false)continue;
		(*b)->DrawFlag = false;
		//:::::::::::ビーム１本分のメッシュ処理::::::::::::::://
		int vnum = (*b)->GetVertexNum();
		FOR(vnum)
		{
			//ビーム内の頂点1つの処理

			//処理する頂点
			vec* nowpos = &(*b)->GetBBBstruct()[i].pos;

			// 点からカメラへの単位ベクトルを求める
			vec	z;
			vec t_cam = XMLoadFloat3(pCamera->GetCameraPointer()->GetCameraPos());
			z = XMVectorSubtract(t_cam, *nowpos);

			// 点の単位接線ベクトルを求める
			vec	t;
			if (i == 0) //見てる点が最初
			{
				t = XMVectorSubtract((*b)->GetBBBstruct()[i + 1].pos, *nowpos);
				//t = m_PointList[i + 1] - m_PointList[i];
			}
			else if (i == vnum - 1) //見てる点が最後
			{
				t = XMVectorSubtract(*nowpos, (*b)->GetBBBstruct()[i - 1].pos);
				//t = m_PointList[i] - m_PointList[i - 1];
			}
			else //見てる点が中間
			{
				t = XMVectorSubtract((*b)->GetBBBstruct()[i + 1].pos, (*b)->GetBBBstruct()[i - 1].pos);
				//t = m_PointList[i + 1] - m_PointList[i - 1];
			}

			// ストリップ頂点を求める
			vec	cross;
			cross = XMVector3Cross(t, z);
			cross = XMVector3Normalize(cross);
			cross = XMVectorScale(cross, width);
			pBBBV[count * 2].pos = XMVectorSubtract(*nowpos, cross);
			pBBBV[count * 2 + 1].pos = XMVectorAdd(*nowpos, cross);

			// UV座標を設定する
			pBBBV[count * 2].u = 0.0f;
			pBBBV[count * 2].v = (float)i / (vnum-1);
			pBBBV[count * 2 + 1].u = 0.125f;
			pBBBV[count * 2 + 1].v = (float)i / (vnum-1);

			++count;
		}
		//::::::::::::::::::::ここまで:::::::::::::::::::://
		ArrIndex[BeamNum++] = count++;
	}
	//ビームとビームの繋ぎ用頂点の設定
	// 頂点を求める 最後の頂点からの繋ぎ用頂点は描画しないので-1
	FOR(BeamNum-1)
	{
		memcpy_s(&pBBBV[ArrIndex[i] * 2].pos, sizeof(vec), &pBBBV[ArrIndex[i] * 2 - 1].pos, sizeof(vec));
		memcpy_s(&pBBBV[ArrIndex[i] * 2 + 1].pos, sizeof(vec), &pBBBV[ArrIndex[i] * 2 + 2].pos, sizeof(vec));
		// UV座標を設定する
		pBBBV[ArrIndex[i] * 2].u = 0.f;
		pBBBV[ArrIndex[i] * 2].v = 0.f;
		pBBBV[ArrIndex[i] * 2 + 1].u = 0.f;
		pBBBV[ArrIndex[i] * 2 + 1].v = 0.f;
	}
	delete [] ArrIndex;
	//pDev->SetVertexDeclaration(pVertDecl);
}


void BBBM::Draw()
{
	if (AllVertexNum == 0)return; //BBBが登録されていなかったら描画しなくていい
	pDataBox->SetTexture(texID);
	//pDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, AllVertexNum * 2 - 4, pBBBV, sizeof(BBBVertex));
	AllVertexNum = 0;
}


/////////////////////////////////////////////////////////////////


BBB::BBB(int vertnum, BillBoardBeamManager* parent)
{
	pParentBBBM = parent;
	VertexNum = vertnum;
	DrawFlag = false;
	pArrVertex = new BBBStruct[vertnum];
	//初期化
	FOR(VertexNum)
	{
		pArrVertex[i].pos = XMVectorReplicate(0.0f);
		pArrVertex[i].v = XMVectorReplicate(0.0f);
	}
}


BBB::~BBB()
{
	SAFE_DELETEARRAY(pArrVertex);
}


void BBB::SetAllVertex(vec *pos)
{
	FOR(VertexNum)
	{
		pArrVertex[i].pos = *pos;
	}
}


void BBB::SetVertexPos(vec *pos)
//PosIndexを加えた座標を入れること
{
	//原点はposをそのまま
	pArrVertex[0].pos = *pos;

	//後続の点は１つ前の点にバネ
	for (int i = 1; i < VertexNum; ++i)
	{
		M_VectorSpring(
			&pArrVertex[i].v,
			&pArrVertex[i - 1].pos,
			&pArrVertex[i].pos,
			10.0, 0.04);
	}
}


void BBB::DrawSet()
//描画の準備通知関数　呼ばれるとBBBMが描画をする
{
	pParentBBBM->SetDrawedVertexNum(VertexNum);
	DrawFlag = true;
}