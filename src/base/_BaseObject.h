#ifndef _BASE_OBJECT_H_
#define _BASE_OBJECT_H_

#include<list>
#include"IncludeAll.h"
#include"SystemPointerBox.h"

// �I�u�W�F�N�g��{�N���X
class _BaseObject : public SystemPointerWrap{
protected:

	int meshID;
	int texID;

	////////////////////
	vec			position;
	POSINDEX	posidx;
	mat			rota;
	float		ratio;
	vec			q;	//QUATERNION

	TCHAR meshFilename[100];
	TCHAR texFilename[100];

	//�q�I�u�W�F�N�g�`��
	void DrawChild();

	//PosIdx���ړ������Ƃ��̃t���O
	BYTE MovIdxFlag;

	BOOL DeleteFlag; //�I�u�W�F�N�g�����̂��m�点�@false�ŏ���

public:

	//�q�I�u�W�F�N�g���X�g
	std::list<sp<_BaseObject>> ChildObjectList;

	_BaseObject();
	virtual ~_BaseObject();
	
	void			SetFileName(TCHAR* mesh,TCHAR *tex);
	virtual bool	LoadFile();

	vec			GetPos();
	POSINDEX*	GetPosIdx();
	mat			GetRota();
	vec			GetDirec();
	vec			GetUp();
	vec			GetPitch();

	BYTE	GetMovIdxFlag(){ return MovIdxFlag; }

	void SetPos(vec *pos);
	void SetPosIdx(POSINDEX *pidx);
	void SetPosIdx(int x,int y,int z);
	void SetDirec(vec *dir);
	void SetUp(vec *up);
	void SetPitch(vec *pitch);
	void SetRatio(float r){ratio = r;}

	void SetRota(mat *rota);

	virtual void Move();
	virtual bool Draw();
	virtual void DebugDraw(){};

	//�q�I�u�W�F�N�g�����X�g�ɓo�^
	virtual size_t RegistObject(sp<_BaseObject> spObj);
	//�I�u�W�F�N�g�̏��n
	virtual void UnderTake( sp<_BaseObject> &spObj);
	
};
	
//BaseObject�Ԃ̋����v�Z�ȗ��֐�
vec* DistanceBetBaseObj(vec* out, _BaseObject* p1, _BaseObject* p2);


#endif