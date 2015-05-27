#include"VirtualController.h"


virtualController::virtualController()
{
	pInput=NULL;

	FOR(PadNum)ConvControllerID[i] = 0;

	for(int i=0;i<128;++i){
		convBotton[i]=i;
	}
	convStick[0]	=_VC_STICK_LY;	//pitch
	convStick[1]	=_VC_STICK_LX;	//roll
	convStick[2]	=_VC_BUTTON_LR;	//yaw

	convCamera[0]	=_VC_STICK_RY;
	convCamera[1]	=_VC_STICK_RX;

	reverseFlag[0]	= 1;
	reverseFlag[1]	= 1;
	reverseFlag[2]	=-1;
	reverseFlag[3]	=-1;

	Load();
}
virtualController::~virtualController()
{
	pInput=NULL;
}

void virtualController::Load()
{
	FILE *fp=NULL;
	fp = fopen("data/Config.txt","r");
	if(fp==NULL || errno != 0){
		MessageBox(NULL,_T("virtualController::Load\nコントローラーの設定を読み込まずに起動します"),_T("message"),MB_OK);
		return;
	}

	char brank[100]={0};
	FOR(128)
	{
		char result = fscanf(fp,"%s : %d",brank, &convBotton[i]);
		convBotton[i]--;
		if(result<=0 || result==EOF)break;
	}
	fclose(fp);
}

void virtualController::reset()
{
	FOR(128)convBotton[i]=i;

	convStick[0]	=_VC_STICK_RY;
	convStick[1]	=_VC_STICK_RX;
	convStick[2]	=_VC_BUTTON_LR;

	convCamera[0]	=_VC_STICK_LY;
	convCamera[1]	=_VC_STICK_LX;
}
void virtualController::setInput(Input* p)
{
	pInput = p;
}


///////////////// メニュー用ボタンセット
BYTE virtualController::OK(BYTE playerNum)
{
	return pInput->GetButtonNow(convBotton[_VC_CIRCLE], playerNumConv)
		|pInput->GetKeyNow(DIK_RETURN);
}
BYTE virtualController::Cancel(BYTE playerNum)
{
	return pInput->GetButtonNow(convBotton[_VC_CROSS], playerNumConv)
		|pInput->GetKeyNow(DIK_BACKSPACE);
}
BYTE virtualController::Up(BYTE playerNum)
{
	return 
		pInput->GetCrossNow(INPUT_CROSS_UP, playerNumConv)
		|pInput->GetKeyNow(DIK_UP);
}
BYTE virtualController::Down(BYTE playerNum)
{
	return 
		pInput->GetCrossNow(INPUT_CROSS_DOWN, playerNumConv)
		|pInput->GetKeyNow(DIK_DOWN);
}
BYTE virtualController::Left(BYTE playerNum)
{
	return 
		pInput->GetCrossNow(INPUT_CROSS_LEFT, playerNumConv)
		|pInput->GetKeyNow(DIK_LEFT);
}
BYTE virtualController::Right(BYTE playerNum)
{
	return 
		pInput->GetCrossNow(INPUT_CROSS_RIGHT, playerNumConv)
		|pInput->GetKeyNow(DIK_RIGHT);
}



///////////////// ゲーム用ボタンセット
BYTE virtualController::Accel(BYTE playerNum)
{
	return pInput->GetButton(convBotton[_VC_R2], playerNumConv);
}
BYTE virtualController::Brake(BYTE playerNum){
	return pInput->GetButton(convBotton[_VC_L2], playerNumConv);
}
//BYTE virtualController::Item(){}
BYTE virtualController::FaceFoward(BYTE playerNum)
{
	//●×ボタン群or十字キー
	if(1){
		return pInput->GetButton(convBotton[_VC_R3], playerNumConv);
		//return pInput->GetCross(INPUT_CROSS_RIGHT,playerNum);
	}else{
		return pInput->GetButton(convBotton[_VC_L3], playerNumConv);
		//return pInput->GetButton(convBotton[3],playerNum);
	}
}
BYTE virtualController::UseItem(BYTE playerNum)
{
	return pInput->GetButton(convBotton[_VC_CIRCLE], playerNumConv);
}
BYTE virtualController::CamChangeNear(BYTE playerNum)
{
	//●×ボタン群or十字キー
	if(0){
		return pInput->GetCrossNow(INPUT_CROSS_UP, playerNumConv);
	}else{
		return pInput->GetButtonNow(convBotton[_VC_TRIANGLE], playerNumConv);
	}
}
BYTE virtualController::CamChangeFar(BYTE playerNum)
{
	//●×ボタン群or十字キー
	if(0){
		return pInput->GetCrossNow(INPUT_CROSS_DOWN, playerNumConv);
	}else{
		return pInput->GetButtonNow(convBotton[_VC_CROSS], playerNumConv);
	}
}
BYTE virtualController::ChangeDriveMode(BYTE playerNum)
{
	return pInput->GetButtonNow(convBotton[_VC_SELECT], playerNumConv);
}

///////////////////// スティックセット
FLOAT virtualController::stickControl(int flag, BYTE playerNum)
{
	int a;
	switch(convStick[flag]){
	case _VC_STICK_LY: return pInput->GetLY(playerNumConv)*0.001f*reverseFlag[0];
	case _VC_STICK_LX: return pInput->GetLX(playerNumConv)*0.001f*reverseFlag[1];
	case _VC_STICK_RY: return pInput->GetRY(playerNumConv)*0.001f*reverseFlag[0];
	case _VC_STICK_RX: return pInput->GetRX(playerNumConv)*0.001f*reverseFlag[1];
	case _VC_BUTTON_LR:
		a = (int)pInput->GetButton(convBotton[_VC_L1], playerNumConv)
			- (int)pInput->GetButton(convBotton[_VC_R1], playerNumConv);
		a/=128*reverseFlag[1];
		return (FLOAT)a;
	case _VC_STICK_NO: return 0.0f;
	}
	return 0.0;
}
FLOAT virtualController::Pitch(BYTE playerNum){
	return stickControl(0, playerNumConv);
}
FLOAT virtualController::Roll(BYTE playerNum){
	return stickControl(1, playerNumConv);
}
FLOAT virtualController::Yaw(BYTE playerNum){
	return stickControl(2, playerNumConv);
}
FLOAT virtualController::Camera(int flag, BYTE playerNum)
{
	switch(convCamera[flag]){
	case _VC_STICK_LY: return pInput->GetLY(playerNumConv)*0.001f*reverseFlag[2];
	case _VC_STICK_LX: return pInput->GetLX(playerNumConv)*0.001f*reverseFlag[3];
	case _VC_STICK_RY: return pInput->GetRY(playerNumConv)*0.001f*reverseFlag[2];
	case _VC_STICK_RX: return pInput->GetRX(playerNumConv)*0.001f*reverseFlag[3];
	case _VC_STICK_NO: return 0.0f;
	}
	return 0.0;
}

FLOAT virtualController::LX(BYTE playerNum){ return pInput->GetLX(playerNumConv)*0.001f; }
FLOAT virtualController::LY(BYTE playerNum){ return pInput->GetLY(playerNumConv)*0.001f; }
FLOAT virtualController::RX(BYTE playerNum){ return pInput->GetRX(playerNumConv)*0.001f; }
FLOAT virtualController::RY(BYTE playerNum){ return pInput->GetRY(playerNumConv)*0.001f; }
BYTE  virtualController::Button(int bnum, BYTE playerNum){ return pInput->GetButton(convBotton[bnum], playerNumConv); }
BYTE  virtualController::ButtonN(int bnum, BYTE playerNum){ return pInput->GetButtonNow(convBotton[bnum], playerNumConv); }
BYTE virtualController::CrossD(int direct, BYTE playerNum){ return pInput->GetCross(direct, playerNumConv); }


int virtualController::JoinNewPlayer(int MaxPlayer)
	//戻り値はインプットのID番号
{
	for(int i=MaxPlayer; i<PadNum; ++i)
	{
		if (OK(i))
		{
			ConvControllerID[MaxPlayer] = i;
			return i;
		}
#ifdef _DEBUG
		if (pInput->GetKeyNow(DIK_J))
		{
			ConvControllerID[MaxPlayer] = i;
			return i;
		}
#endif
	}
	return 0;
}

