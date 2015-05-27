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

	//�e�N�X�`���ǂݍ���
	pDataBox = pdb;
	texID = pdb->GetTexture(LOAD_T_BEAM);

	// ���_�錾���쐬����
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

	if (VertexNum <= 1)	//���_�����P�ȉ����ƃ��b�V�������Ȃ��̂ŃG���[
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
	if (AllVertexNum == 0)return; //BBB���o�^����Ă��Ȃ�������`�悵�Ȃ��Ă���

	//���b�V���f�[�^���쐬����
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
		//:::::::::::�r�[���P�{���̃��b�V������::::::::::::::://
		int vnum = (*b)->GetVertexNum();
		FOR(vnum)
		{
			//�r�[�����̒��_1�̏���

			//�������钸�_
			vec* nowpos = &(*b)->GetBBBstruct()[i].pos;

			// �_����J�����ւ̒P�ʃx�N�g�������߂�
			vec	z;
			vec t_cam = XMLoadFloat3(pCamera->GetCameraPointer()->GetCameraPos());
			z = XMVectorSubtract(t_cam, *nowpos);

			// �_�̒P�ʐڐ��x�N�g�������߂�
			vec	t;
			if (i == 0) //���Ă�_���ŏ�
			{
				t = XMVectorSubtract((*b)->GetBBBstruct()[i + 1].pos, *nowpos);
				//t = m_PointList[i + 1] - m_PointList[i];
			}
			else if (i == vnum - 1) //���Ă�_���Ō�
			{
				t = XMVectorSubtract(*nowpos, (*b)->GetBBBstruct()[i - 1].pos);
				//t = m_PointList[i] - m_PointList[i - 1];
			}
			else //���Ă�_������
			{
				t = XMVectorSubtract((*b)->GetBBBstruct()[i + 1].pos, (*b)->GetBBBstruct()[i - 1].pos);
				//t = m_PointList[i + 1] - m_PointList[i - 1];
			}

			// �X�g���b�v���_�����߂�
			vec	cross;
			cross = XMVector3Cross(t, z);
			cross = XMVector3Normalize(cross);
			cross = XMVectorScale(cross, width);
			pBBBV[count * 2].pos = XMVectorSubtract(*nowpos, cross);
			pBBBV[count * 2 + 1].pos = XMVectorAdd(*nowpos, cross);

			// UV���W��ݒ肷��
			pBBBV[count * 2].u = 0.0f;
			pBBBV[count * 2].v = (float)i / (vnum-1);
			pBBBV[count * 2 + 1].u = 0.125f;
			pBBBV[count * 2 + 1].v = (float)i / (vnum-1);

			++count;
		}
		//::::::::::::::::::::�����܂�:::::::::::::::::::://
		ArrIndex[BeamNum++] = count++;
	}
	//�r�[���ƃr�[���̌q���p���_�̐ݒ�
	// ���_�����߂� �Ō�̒��_����̌q���p���_�͕`�悵�Ȃ��̂�-1
	FOR(BeamNum-1)
	{
		memcpy_s(&pBBBV[ArrIndex[i] * 2].pos, sizeof(vec), &pBBBV[ArrIndex[i] * 2 - 1].pos, sizeof(vec));
		memcpy_s(&pBBBV[ArrIndex[i] * 2 + 1].pos, sizeof(vec), &pBBBV[ArrIndex[i] * 2 + 2].pos, sizeof(vec));
		// UV���W��ݒ肷��
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
	if (AllVertexNum == 0)return; //BBB���o�^����Ă��Ȃ�������`�悵�Ȃ��Ă���
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
	//������
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
//PosIndex�����������W�����邱��
{
	//���_��pos�����̂܂�
	pArrVertex[0].pos = *pos;

	//�㑱�̓_�͂P�O�̓_�Ƀo�l
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
//�`��̏����ʒm�֐��@�Ă΂���BBBM���`�������
{
	pParentBBBM->SetDrawedVertexNum(VertexNum);
	DrawFlag = true;
}