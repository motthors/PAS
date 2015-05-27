#ifndef _BBBM_H_
#define _BBBM_H_

#include"IncludeAll.h"
#include"DataBox.h"
#include"CameraController.h"
#include<list>

class BBB;

class BillBoardBeamManager {

	///DirectX///
	//���_�錾
	typedef struct{
		vec		pos;	// ���W
		float	u, v;	// UV���W
	} BBBVertex;
	int texID;
	LPDIRECT3DVERTEXDECLARATION9	pVertDecl;
	/////////////

	BBBVertex* pBBBV;	//���b�V���i�[�̈�ւ̃|�C���^

	DXDEVICE*	pDev;
	DataBox*	pDataBox;
	CameraController* pCamera;


	std::list<BBB*> listBBBArr;
	int AllVertexNum;	//���_�f�[�^�̕K�v�����@DrawCall���P��ōς܂�����
	float width;		//�r�[���̑���
public:
	
	BillBoardBeamManager();
	virtual ~BillBoardBeamManager();

	int Init(DXDEVICE* pdev, DataBox* pdb, CameraController* pcam);

	BBB* CreateBBB(int VertexNum); //�r�[���P�{���̃}�l�[�W���[
	void DeleteBBB(BBB* p);			//�r�[������

	void SetDrawedVertexNum(int num);
	void MeshSet();	//���b�V���̓W�J�v�Z
	void Draw();		//�}�l�[�W�����̂��ׂẴr�[���̕`��
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

	void DrawSet();	//�`��̏����ʒm�֐�
};


#endif