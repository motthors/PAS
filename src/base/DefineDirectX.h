#ifndef _DEFINE_DIRECTX_
#define _DEFINE_DIRECTX_


#include <D3D11.h>
#include <d3d9.h>
#include <DirectXMath.h>

#pragma comment(lib,"d3d11.lib")


#define using_dx11
#ifdef using_dx11
#define DXDEVICE	ID3D11Device
#define DXCONTEXT	ID3D11DeviceContext
#define DXSC		IDXGISwapChain

#define DXTEXTURE2D	ID3D11Texture2D
#else
#define DXDEVICE	ID3D9Device
#define DXCONTEXT	ID3D9Device
#endif
#endif