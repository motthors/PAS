#ifndef _LOG_MANAGER_H_
#define _LOG_MANAGER_H_

/////////////////////////////////////////
// ���O�}�l�[�W��
// ����Ă������O�̊Ǘ��Ə������̕\���Ǘ�
/////////////////////////////////////////


#include"../base/IncludeAll.h"
#include"../base/DebugCommentManager.h"

#define LM_MAXCHAR 100

class LogManager{

	DebugCommentManager* pDebugComM;
	TCHAR **pLogList; //���O�����񃊃X�g
	int lognum;	//���݂̃��O�s�̕ێ���
	int nowidx; //���O���L�^����ʒu�̃C���f�b�N�X
	float percentage; //�ŐV�̃��O�ɂ��Ă̏�����������
public:
	LogManager(DebugCommentManager* pDCM);
	~LogManager();

	int Init(int num);
	void Clear();
	void AddComment(TCHAR* addtext);
	void NowPercentage(float val);
	void Draw();
};

#endif