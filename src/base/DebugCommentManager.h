#ifndef DEBUG_COMMENT_MANAGER_H_
#define DEBUG_COMMENT_MANAGER_H_
//////////////////////////////////////////
//�f�o�b�O�R�����g�\���N���X
//��ʂ�NxN�ɕ������A�����Ń}�X������U��`��
////////////////////////////////////////


#include"IncludeAll.h"

#include"SpriteBatch.h"
#include"SpriteFont.h"

using namespace DirectX;

#define TEXT_BOX_SIZE 1000

class DebugCommentManager {

	//DXTK
	SpriteBatch*	m_pSpriteBatch;
	SpriteFont*		m_pSpriteFont;

	int DivWindowNum;
	TCHAR **Text;
	int index;

	int WindowDivX, WindowDivY;

	//�J���Ă�}�X��T��
	int SearchFreeBox();
public:
	DebugCommentManager( DXDEVICE* pDevice, DXCONTEXT* pContext);
	~DebugCommentManager();

	void SetDivideNum(int divnum, int WindowX, int WindowY);

	void Reset();
	//void BeginSet();
	void Set(TCHAR *src, int idx=-1);
	void End();

	void Draw();
};

#endif