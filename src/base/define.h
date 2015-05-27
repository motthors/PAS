#ifndef _DEFINE_H_
#define _DEFINE_H_

//const
#define WINDOW_SIZE_W 1240
#define WINDOW_SIZE_H 720

#define WINDOW_SIZEH_W 620
#define WINDOW_SIZEH_H 360

#define twoPHI	6.2831853f
#define PHI		3.141592f
#define hPHI	1.57078f

#define MYTIME float

#define _GAMENAME "mottygame2"

#define PadNum 4
#define MAX_MACHINE_NUM 30

//posision index
typedef __int64 ONEOF_PI;
typedef struct{ONEOF_PI x;ONEOF_PI y;ONEOF_PI z;} POSINDEX;
#define POSIDX_LENGTH 1000.f


//macro
#define FOR(x) for(int i=0;i<x;++i)
#define FOR2(x) for(int j=0;j<x;++j)

#define RELEASE(x)				if(x){x->Release();x=NULL;}
#define SAFE_DELETE(x)			if(x){delete (x);(x)=NULL;}
#define SAFE_DELETEARRAY(x)		if(x){delete[] (x);(x)=NULL;}

#define ALERT(x)				MessageBox(NULL,x,_T("error"),MB_OK)
#define ALERTa(x)				MessageBox(NULL,x,"error",MB_OK)


//vector define
#define vec		XMVECTOR
#define vec2	XMFLOAT2
#define vec3	XMFLOAT3
#define vec4	XMFLOAT4

#define mat		XMMATRIX
#define mat4	XMFLOAT4X4

#endif
