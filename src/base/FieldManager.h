#ifndef _FIELD_MANAGER_H_
#define _FIELD_MANAGER_H_
/*//////////////////////////////////////////////////
			�t�B�[���h�}�l�[�W���N���X
�I�[�v�����[���h��z�肵���t�B�[���h�����N���X
�`��̗L���̌���Ȃǂ̃}�l�W�����g
�����Ȃ��I�u�W�F�N�g�Ɠ����I�u�W�F�N�g��ʁX�ɊǗ�
�i�����I�u�W�F�N�g�͐�p�̃N���X������j
��������\��Ƃ��āA�����ōׂ����I�u�W�F�N�g�̔z�u�Ȃ�
//////////////////////////////////////////////////////*/
class FieldManager;

#include"IncludeAll.h"
#include"SystemPointerBox.h"

#include"sFieldObject.h"

#define FIELD_OBJ_NUM 3

class FieldManager : public SystemPointerWrap{

	sFieldObject *SpaceDome;
	sFieldObject *Planet;
	std::list<sp<sFieldObject>> ObjectList;

public:
	FieldManager();
	~FieldManager();

	int Init();

	void SetPosIdx(POSINDEX* pidx);
	void Move();
	void Draw();
};

#endif
