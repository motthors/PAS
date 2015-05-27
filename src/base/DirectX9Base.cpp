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

	// Direct3D�̏�����
	if( !(pD3D = Direct3DCreate9( D3D_SDK_VERSION )) ) 
	   return -3;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	D3DDISPLAYMODE d3ddm;
	// ���݂̃f�B�X�v���C���[�h���擾
    if(FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
		return -4;

	d3dpp.BackBufferCount				= 1;//�o�b�N�o�b�t�@�̖���
	d3dpp.BackBufferFormat				= d3ddm.Format;
	d3dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;//�}���`�T���v���̃��x���ݒ�
	d3dpp.MultiSampleQuality			= 0;//g_pD3D->CheckDeviceMultiSampleType(,
	d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;//�t�����g�o�b�t�@�ƃo�b�N�o�b�t�@�̐؂�ւ����@
	d3dpp.hDeviceWindow					= hWnd;
	d3dpp.Windowed						= flscflag?FALSE:TRUE;
	d3dpp.EnableAutoDepthStencil		= TRUE;
	d3dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;
	d3dpp.Flags							= 0;
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;//�X���b�v�̏��������^�C�~���O�@D3DPRESENT_INTERVAL_DEFAULT���w�肷��ƁA�����A���Ԋu��҂� D3DPRESENT_INTERVAL_IMMEDIATE�ő҂��Ȃ�
	d3dpp.BackBufferWidth				= DefRender.WindowX;
	d3dpp.BackBufferHeight				= DefRender.WindowY;

   
	if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pd3ddev ) ) ){
		if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pd3ddev ) ) ){
			if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pd3ddev ) ) ){
				if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pd3ddev ) ) ){
					MessageBox(hWnd,_T("DirectX�̃f�o�C�X�擾�Ɏ��s���܂���"),_T("DirectX Init Error"),MB_OK);
      				return -5;
				}
			}
		}
	}

	//�}���`�����_�����O�^�[�Q�b�g�̍ő吔�`�F�b�N
	D3DCAPS9 Caps;
	pd3ddev->GetDeviceCaps( &Caps );
	DWORD MaxRT = Caps.NumSimultaneousRTs;
	//if( DefRender.RenderTargetNum > MaxRT ) 
		return -6;

	//////////////////////////////////////////////////////////////////

	//// �f�o�b�O�p�t�H���g
	//D3DXFONT_DESC d3df = {
	//	18, //�t�H���g�T�C�Y
	//	0, 
	//	0, 
	//	1, 
	//	0, 
	//	SHIFTJIS_CHARSET, 
	//	OUT_TT_ONLY_PRECIS,
	//	PROOF_QUALITY, 
	//	
	//	FIXED_PITCH | FF_MODERN, 
	//	_T("���C���I")
	//};

	//// ID3DXFont�R���|�[�l���g
	//if(FAILED(D3DXCreateFontIndirect(pd3ddev, &d3df, &pd3dfont))){
	//	return -5;
	//}

	///////////////////////////////////////////////////////////////////

	//// ���C�g
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
	
	// �����_�����O�X�e�[�g
	//RGB�J���[�v�f�̐ݒ�
	pd3ddev->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
    pd3ddev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pd3ddev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	//�A���t�@�v�f�̐ݒ�
	pd3ddev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
	pd3ddev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pd3ddev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	// �`�惂�[�h�̎w��
	// �y��r���s�Ȃ�
    pd3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
	// �f�B�U�����O���s�Ȃ��i���i���`��j
	pd3ddev->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	//�A���t�@�u�����h���s��
	pd3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//�������������s��
	pd3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//���ߏ������s��
	pd3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//�w�ʃJ�����O���s��
	pd3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	pd3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pd3ddev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pd3ddev->SetRenderState(D3DRS_ALPHAREF, 1);

	pd3ddev->SetRenderState(D3DRS_AMBIENT, 0x33333333);
	pd3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	
	//�e�N�X�`���t�B���^
	pd3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pd3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//�e�N�X�`���̃A�h���b�V���O
	pd3ddev->SetSamplerState(0, D3DSAMP_ADDRESSU , D3DTADDRESS_WRAP);
	pd3ddev->SetSamplerState(0, D3DSAMP_ADDRESSV , D3DTADDRESS_WRAP);
	
	//���C�g�̃f�t�H���g�ݒ�
	pd3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);

	//���C���[�t���[���e�X�g
	//pd3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//�V�F�[�_�[���[�h�̕ύX�i�O�[���[
	//pd3ddev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	////�e�N�X�`���X�e�[�W�̃A��
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
//		MessageBox(NULL,_T("���Ԃ�f�o�C�X���X�g���܂����I �\�t�g���I�����܂��傤�I�I"),_T("error"),MB_OK);
//	}
//}