#include"MatrixBox.h"


MatrixBoxManager::MatrixBoxManager()
{
	FOR(2)pMB[i]		= new MatrixBox[MAX_OBJNUM];
	ConstructInit();
}


MatrixBoxManager::MatrixBoxManager(int ObjNum)
{
	FOR(2)pMB[i]		= new MatrixBox[ObjNum];
	ConstructInit();
}


void MatrixBoxManager::ConstructInit()
{
	FOR(2)pMBT[i]		= new MatrixBoxTime[MAX_OBJNUM_T];
	FOR(MATBOX_TOTALNUM)pSet[i] = NULL;

	BoxIdx	= 0;
	TimeIdx	= 0;
	StateIndex = 0;
}


MatrixBoxManager::~MatrixBoxManager()
{
	FOR(2)SAFE_DELETEARRAY(pMB[i]);	
	FOR(2)SAFE_DELETEARRAY(pMBT[i]);
}


void MatrixBoxManager::BeginSet()
{
	ChangeStateIndex();
	BoxIdx	= 0;
	TimeIdx	= 0;
	FOR(MATBOX_TOTALNUM)pSet[i] = NULL;
}


void MatrixBoxManager::ChangeStateIndex()
{
	StateIndex = (BYTE)!StateIndex;
}


MatrixBox* MatrixBoxManager::GetEndOfPtr(MatrixBox* ptr)
{
	while(ptr->next != NULL)
	{
		ptr = ptr->next;
	}
	return ptr;
}


void MatrixBoxManager::ConectPointer(int FLAG)
{
	if(pSet[FLAG]==NULL)
	{
		pSet[FLAG] = &pMB[StateIndex][BoxIdx];
	}
	else
	{
		//switch(FLAG)
		//{
		//case MATBOX_EFECT:
		//case MATBOX_SemiTrans:

		//	break;
		//default:
			GetEndOfPtr(pSet[FLAG])->next = &pMB[StateIndex][BoxIdx];
		//	break;
		//}
	}
	BoxIdx+=1;
}


mat4* MatrixBoxManager::GetNowMatrixPointer()
{
	return &pMB[StateIndex][BoxIdx].mat;
}


void MatrixBoxManager::SetMatrix(
	POSINDEX *posidx,
	int objnum,
	int texnum,
	int FLAG,
	int meshidx
	)
{
#ifdef _DEBUG
	if(texnum == -1)ALERT(_T("MatrixBox:テクスチャが指定されていない"));
#endif
	if(MAX_OBJNUM < BoxIdx) return;

	pMB[StateIndex][BoxIdx].ObjNum	= objnum;
	pMB[StateIndex][BoxIdx].TexNum	= texnum;
	pMB[StateIndex][BoxIdx].MeshIdx	= meshidx;
	pMB[StateIndex][BoxIdx].next	= NULL;
	pMB[StateIndex][BoxIdx].prev	= NULL;
	pMB[StateIndex][BoxIdx].parent	= NULL;

	TransByPosidex(&pMB[StateIndex][BoxIdx].mat, posidx);

	ConectPointer(FLAG);
}


void MatrixBoxManager::SetMatrix(
	mat4 *mat,
	POSINDEX *posidx,
	int objnum,
	int texnum,
	int FLAG,
	int meshidx
	)
{
	pMB[StateIndex][BoxIdx].mat = *mat;
	SetMatrix( posidx, objnum, texnum, FLAG, meshidx);
}


void MatrixBoxManager::SetTime(mat4 *mat, DWORD Time)
{
	if(MAX_OBJNUM < TimeIdx) return;

	pMBT[StateIndex][TimeIdx].mat = *mat;
	pMBT[StateIndex][TimeIdx].Time = Time;
}


mat4* MatrixBoxManager::ConvertBillBoardMatrix(
	mat4* mat,
	vec* pos,
	POSINDEX* idx,
	float scale, 
	bool NoProj
	)
{
	mat4 matScale;
	vec temp;

	Camera* pcam = pCamera->GetCameraPointer();

	POSINDEX* pCameraidx = pcam->GetPosIndex();
	POSINDEX dist;
	PosIdxSub(&dist, idx, pCameraidx);
	//PosIdx2Pos(&temp, pos, &dist);
	//temp.x = pos->x + (idx->x - pCameraidx->x)*POSIDX_LENGTH;
	//temp.y = pos->y + (idx->y - pCameraidx->y)*POSIDX_LENGTH;
	//temp.z = pos->z + (idx->z - pCameraidx->z)*POSIDX_LENGTH;


	if(NoProj)
	{ //どれだけ離れていても等倍ビルボード
		//temp = XMVectorSubtract(*pcam->GetCameraPos(), temp);
		//scale = scale * XMVectorGetX(XMVector3Length(temp));
	}
	//matScale = XMMatrixScaling(scale, scale, scale);
	//*mat = XMMatrixTranspose(*pcam->GetView());
	//M_MatrixOffset(mat, &XMVectorSet(0, 0, 0, 0));
	////mat->_14=0.f;
	////mat->_24=0.f;
	////mat->_34=0.f;
	//M_MatrixOffset(mat,pos);
	//*mat = XMMatrixMultiply(matScale, *mat);
	return mat;
}


void MatrixBoxManager::TransByPosidex(mat4* mat, POSINDEX *idx)
{
	POSINDEX *temp = pCamera->GetCameraPointer()->GetPosIndex();
	POSINDEX dist;
	PosIdxSub(&dist, idx, temp);
	vec posdist = XMVectorSet(
		(float)(idx->x - temp->x) * POSIDX_LENGTH,
		(float)(idx->y - temp->y) * POSIDX_LENGTH,
		(float)(idx->z - temp->z) * POSIDX_LENGTH,
		0.f
	);
	//M_MatrixOffset(mat, &posdist);
}
