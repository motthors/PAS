#include"LogManager.h"


LogManager::LogManager(DebugCommentManager* pDCM)
{
	pDebugComM = pDCM;
	pLogList = nullptr;
	lognum = 0;
	nowidx = 0;
	percentage = 0;
}

LogManager::~LogManager()
{
	FOR(lognum)SAFE_DELETEARRAY(pLogList[i]);
	SAFE_DELETEARRAY(pLogList);
}

int LogManager::Init(int num)
//�Ǘ����郍�O�̗ʂ̐ݒ�
{
	if (num < 1)return -1; //�}�C�i�X�̍s����邱�Ƃ͂ł��Ȃ��̂ŃG���[
	if (num > 30)return -1; //�ő�l��30�܂łƂ���

	pLogList = new TCHAR*[num];
	FOR(num)
	{
		pLogList[i] = new TCHAR[LM_MAXCHAR]; //1�s��100�����܂łƂ���
		FOR2(LM_MAXCHAR)pLogList[i][j] = 0;
	}
	lognum = num;
	nowidx = 0;
	percentage = 0;
	return 0;
}


void LogManager::Clear()
{
	FOR(lognum)
	{
		pLogList[i][0] = 0;
	}
}


void LogManager::AddComment(TCHAR* addtext)
//���O���X�g�Ƀe�L�X�g��ǉ�
{
	_tcscpy_s(pLogList[nowidx], LM_MAXCHAR, addtext);
	nowidx = (++nowidx) % lognum;
	percentage = 0;
}


void LogManager::NowPercentage(float val)
{
	percentage = val;
}


void LogManager::Draw()
{
	int tempidx = (nowidx) % lognum;
	pDebugComM->Set(pLogList[(tempidx) % lognum], 14);
	FOR(lognum-1)
	{
		pDebugComM->Set(_T("\n"), 14);
		pDebugComM->Set(pLogList[(tempidx + i+1) % (lognum)], 14);
	}
	TCHAR text[20];
	if (percentage != 0){
		_stprintf_s(text, 19, _T(" ... %.1f"), percentage);
		pDebugComM->Set(text, 14);
	}
	pDebugComM->Set(_T("\n"), 14);

	pDebugComM->End();
}