#include"DirectX9Base.h"
#include"DefineRendering.h"

DirectX9Base::DirectX9Base()
{
	pD3D		=0;
	pd3ddev		=0;
	//pd3dfont	=0;
	//pEffect	=0;
}

DirectX9Base::~DirectX9Base()
{
	//RELEASE(pd3dfont);
	RELEASE(pd3ddev);
	RELEASE(pD3D);
}

HRESULT DirectX9Base::Init(HWND hWnd, bool flscflag)
{

	// Direct3Dの初期化
	if( !(pD3D = Direct3DCreate9( D3D_SDK_VERSION )) ) 
	   return -3;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	D3DDISPLAYMODE d3ddm;
	// 現在のディスプレイモードを取得
    if(FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
		return -4;

	d3dpp.BackBufferCount				= 1;//バックバッファの枚数
	d3dpp.BackBufferFormat				= d3ddm.Format;
	d3dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;//マルチサンプルのレベル設定
	d3dpp.MultiSampleQuality			= 0;//g_pD3D->CheckDeviceMultiSampleType(,
	d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;//フロントバッファとバックバッファの切り替え方法
	d3dpp.hDeviceWindow					= hWnd;
	d3dpp.Windowed						= flscflag?FALSE:TRUE;
	d3dpp.EnableAutoDepthStencil		= TRUE;
	d3dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;
	d3dpp.Flags							= 0;
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;//スワップの書き換えタイミング　D3DPRESENT_INTERVAL_DEFAULTを指定すると、垂直帰線間隔を待つ D3DPRESENT_INTERVAL_IMMEDIATEで待たない
	d3dpp.BackBufferWidth				= DefRender.WindowX;
	d3dpp.BackBufferHeight				= DefRender.WindowY;

   
	if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pd3ddev ) ) ){
		if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pd3ddev ) ) ){
			if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pd3ddev ) ) ){
				if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pd3ddev ) ) ){
					MessageBox(hWnd,_T("DirectXのデバイス取得に失敗しました"),_T("DirectX Init Error"),MB_OK);
      				return -5;
				}
			}
		}
	}

	//マルチレンダリングターゲットの最大数チェック
	D3DCAPS9 Caps;
	pd3ddev->GetDeviceCaps( &Caps );
	DWORD MaxRT = Caps.NumSimultaneousRTs;
	//if( DefRender.RenderTargetNum > MaxRT ) 
		return -6;

	//////////////////////////////////////////////////////////////////

	//// デバッグ用フォント
	//D3DXFONT_DESC d3df = {
	//	18, //フォントサイズ
	//	0, 
	//	0, 
	//	1, 
	//	0, 
	//	SHIFTJIS_CHARSET, 
	//	OUT_TT_ONLY_PRECIS,
	//	PROOF_QUALITY, 
	//	
	//	FIXED_PITCH | FF_MODERN, 
	//	_T("メイリオ")
	//};

	//// ID3DXFontコンポーネント
	//if(FAILED(D3DXCreateFontIndirect(pd3ddev, &d3df, &pd3dfont))){
	//	return -5;
	//}

	///////////////////////////////////////////////////////////////////

	//// ライト
	//ZeroMemory(&light, sizeof(D3DLIGHT9) );
	//light.Direction = (-10, -20, 0);
	//light.Type = D3DLIGHT_DIRECTIONAL; 
	//light.Diffuse.r = 1.0f;
	//light.Diffuse.g = 1.0f;
	//light.Diffuse.b = 1.0f;

	//light.Specular.r = 10.0f;
	//light.Specular.g = 10.0f;
	//light.Specular.b = 10.0f;

	//light.Ambient.r = 0.5f;
	//light.Ambient.g = 0.5f;
	//light.Ambient.b = 0.5f;
	//light.Range = 1000;

	//pd3ddev->SetLight( 0, &light );
	//pd3ddev->LightEnable( 0, true );

	//////////////////////////////////////////////////////////////////
	
	// レンダリングステート
	//RGBカラー要素の設定
	pd3ddev->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
    pd3ddev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pd3ddev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	//アルファ要素の設定
	pd3ddev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
	pd3ddev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pd3ddev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	// 描画モードの指定
	// Ｚ比較を行なう
    pd3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
	// ディザリングを行なう（高品質描画）
	pd3ddev->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	//アルファブレンドを行う
	pd3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//半透明処理を行う
	pd3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//透過処理を行う
	pd3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//背面カリングを行う
	pd3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	pd3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pd3ddev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pd3ddev->SetRenderState(D3DRS_ALPHAREF, 1);

	pd3ddev->SetRenderState(D3DRS_AMBIENT, 0x33333333);
	pd3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	
	//テクスチャフィルタ
	pd3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pd3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//テクスチャのアドレッシング
	pd3ddev->SetSamplerState(0, D3DSAMP_ADDRESSU , D3DTADDRESS_WRAP);
	pd3ddev->SetSamplerState(0, D3DSAMP_ADDRESSV , D3DTADDRESS_WRAP);
	
	//ライトのデフォルト設定
	pd3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);

	//ワイヤーフレームテスト
	//pd3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//シェーダーモードの変更（グーロー
	//pd3ddev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	////テクスチャステージのアレ
	//pd3ddev->SetTextureStageState(0,	D3DTSS_COLORARG1	, D3DTA_TEXTURE	);
	//pd3ddev->SetTextureStageState(0,	D3DTSS_COLOROP	, D3DTOP_MODULATE	);
	//pd3ddev->SetTextureStageState(0,	D3DTSS_COLORARG2	, D3DTA_DIFFUSE	);
	//pd3ddev->SetTextureStageState(0,	D3DTSS_ALPHAARG1	, D3DTA_TEXTURE	);
	//pd3ddev->SetTextureStageState(0,	D3DTSS_ALPHAOP	, D3DTOP_SELECTARG1	);

	pd3ddev->ShowCursor(1);

	return 1;
}

//
//void DirectX9Base::BeginDraw()
//{
//	pd3ddev->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,50), 1.0f, 0 );
//	pd3ddev->BeginScene();
//}
//
//
//void DirectX9Base::EndDraw()
//{
//	pd3ddev->EndScene();
//	if ( pd3ddev->Present(0, 0, 0, 0) == D3DERR_DEVICELOST ){
//		MessageBox(NULL,_T("たぶんデバイスロストしました！ ソフトを終了しましょう！！"),_T("error"),MB_OK);
//	}
//}