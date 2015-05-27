#ifndef _CPO_H_
#define _CPO_H_

#include"IncludeAll.h"

#define CPO_HIDE	0
#define CPO_START	1
#define CPO_GOAL	2


typedef struct {
public:
	D3DXMATRIX worldMat;
	int ID;				//�X�e�[�W����ID 0~10(�܂����߂ĂȂ�) �ŗLID 100~
	
} CO;

class CPO{
public:

	D3DXVECTOR3 position;
	int x,y,z;
	D3DXMATRIX  rota;
	float		ratio;
	//D3DXQUATERNION q;

	int pointFlag;		//0:�����Ȃ��|�C���g 1:�X�^�[�g�n�_ 2:�S�[�� 3:�S�[����^�C�����ʕ\���ʒu�@n:�X�e�[�W�ŗL�|�C���gID
	int ID;				//�`�F�b�N�|�C���g��ID 1�Ԃ���
	int nextPointID;	//�����ǂ��̃|�C���g�ɂȂ��邩
	int prevPointID;	//�O�͂ǂ̃|�C���g�ɂȂ���Ă邩
	int blanchNum;		//�����ꓹ�̐�
	int connectFlag;	//�|�C���g�Ԃ̃K�C�h�̂Ȃ��� 0:�`���Ȃ� n:�X�e�[�W�ŗL���C��ID�̎w�� 
	FLOAT connectPower;	//�ėp�ϐ� �X�v���C���̊�x�N�g���̑傫���Ƃ���

	int rotaflag;		//��]�s��̕ύX���������ǂ���

	int objectNum;		//���̃|�C���g�Ɋ܂܂�Ă��锻��L��I�u�W�F�N�g�̐�
	CO* pCOArray;

	int GuideNum;		//���̃|�C���g����L�т�K�C�h�̐�
	D3DXVECTOR3* pGuidePos;	//�K�C�h�̈ʒu�̃|�C���^

	CPO(D3DXVECTOR3 pos=D3DXVECTOR3(0.0f,0.0f,0.0f)){
		position = pos;
		x=y=z=0;
		D3DXMatrixIdentity(&rota);
		ratio			= 70.0f;
		//D3DXQuaternionIdentity(&q);

		pointFlag		= 0;	
		ID				= -1;			
		nextPointID		= -1;
		prevPointID		= -1;
		blanchNum		= false;
		connectFlag		= 1;
		connectPower	= 1;
		rotaflag		= 0;
		objectNum		= 0;
		pCOArray		= NULL;
		GuideNum		= 0;
		pGuidePos		= NULL;
	};

};

#endif