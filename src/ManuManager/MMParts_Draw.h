#ifndef _MMPARTS_DRAW_H_
#define _MMPARTS_DRAW_H_

#include"../base/IncludeAll.h"
#include"../2DDrawer/2DDrawManager.h"
#include"../base/LinerAdder.h"

#include"MenuManager_Parts.h"

///////////////////////////////////////////////////
//�`��ɕK�v�ȏ�����S������N���X

class MMParts_Draw : public MMParts_Base{
public:
	My2DDrawManager* pDrawer;
	LinerAdder* plaSize;
	float	x_Dir;		//��яo�����삷��Ƃ��̈ړ�����
	float	y_Dir;
	float	x_texel;	//��яo���̊���W�e�N�Z���
	float	y_texel;
	float	size = 0.2f;//temp
	float	alpha = 1.0f;
	int		texidName = -1;
	float	texwidthName = 0;
	int		texidImage = -1;
	float	AspectDistortion;

	MMParts_Draw();
	virtual ~MMParts_Draw();

	void Init(int texidName, int texidImage);
	void Set2DDrawer(My2DDrawManager* p){ pDrawer = p; }
	virtual void SetPos(float startx, float starty, float dirx, float diry, float aspect);
	virtual BYTE SelectJudge(float x, float y, float AspectDistortion);

	virtual void Appear();
	virtual void Select();
	virtual bool Cancel();
	virtual void AllCancel(){}
	
	void Inactive();
	void Active();
	void Drag(float x, float y);

	virtual void Move();
	virtual void Draw();
};	


//////////////////////////////////////////////////////
//	PAS�̃��j���[�p�h���N���X

class MMParts_Root : public MMParts_Draw{
public:
	//variable

	//function
	MMParts_Root();
	virtual ~MMParts_Root();

	virtual void Appear();
	//virtual void Select(); //root�Ȃ�Select()�͌Ă΂�Ȃ��@�͂�
	virtual bool Cancel();
	virtual void AllCancel();

	virtual void Move();
	virtual void Draw();
};


class MMParts_Folder : public MMParts_Root{
public:
	virtual void Appear();
	void Select(); //�I���̍��ڂ̂݋������ʁ@Root��Cancel�@Floder�͎q���j���[�W�J
	bool Cancel();
};

class MMParts_Leaf : public MMParts_Folder{
	//���܂̂Ƃ���ύX�͂Ȃ��@���O�����̈Ⴂ
};


#endif