#ifndef _TIME_MEASURER_H_
#define _TIME_MEASURER_H_

#include"IncludeAll.h"


class TimeMeasurer{

	bool flag;	//�^�C�}�[�������Ă��邩�ǂ���
	DWORD time;
	DWORD save;

public:
	TimeMeasurer();
	~TimeMeasurer();

	void Start();	//�v���X�^�[�g(�ăX�^�[�g)
	void Stop();	//�v�����f
	void Reset();	//���Z�b�g

	DWORD GetTime();	//�^�C�}�[�̌��݂̒l�擾
};

#endif