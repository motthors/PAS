#ifndef _LOG_MANAGER_H_
#define _LOG_MANAGER_H_

/////////////////////////////////////////
// ログマネージャ
// 流れていくログの管理と処理率の表示管理
/////////////////////////////////////////


#include"../base/IncludeAll.h"
#include"../base/DebugCommentManager.h"

#define LM_MAXCHAR 100

class LogManager{

	DebugCommentManager* pDebugComM;
	TCHAR **pLogList; //ログ文字列リスト
	int lognum;	//現在のログ行の保持量
	int nowidx; //ログを記録する位置のインデックス
	float percentage; //最新のログについての処理完了割合
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