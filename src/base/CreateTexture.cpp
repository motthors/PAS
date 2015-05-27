#include"CreateTexture.h"


CreateTexture::CreateTexture()
{
	m_pDevice = NULL;
	m_pContext = NULL;
	hFont	= NULL;
	m_FontSize = 0;
}
CreateTexture::~CreateTexture()
{

	DeleteObject(hFont);
}

void CreateTexture::Init(int fontsize){
	//�e�N�X�`���p�t�H���g����
	// �t�H���g�̐���
	LOGFONT lf = {
		fontsize,
		0,							// �����Z���܂��͕����̍���				
		0,							// ���ϕ�����
		0,							// ��������̕�����X���Ƃ̊p�x
		0,							// �x�[�X���C����X���Ƃ̊p�x
		0,							// �t�H���g�̑���
		0,							// �C�^���b�N�̎w��
		0,							// �����t���w��
		DEFAULT_CHARSET,			// �L�����N�^�Z�b�g
		OUT_TT_ONLY_PRECIS,			// �o�͐��x
		CLIP_DEFAULT_PRECIS,		// �N���b�s���O�̐��x
		CLEARTYPE_QUALITY,			// �o�͕i��
		DEFAULT_PITCH | FF_DONTCARE,// �s�b�`�ƃt�@�~��
		_T("MS �S�V�b�N")			// �t�H���g��
	};

	if(!(hFont = CreateFontIndirect(&lf)))
	{
		ErrM.SetClassName(_T("CreateTexture::Init::pCreateTexture::CreateFontIndirect"));
		ErrM.SetErrorText(_T("�t�H���g�f�o�C�X�����Ɏ��s"));
		ErrM.SetHResult(USER_ERROR);
		throw &ErrM;
	}
	m_FontSize = fontsize;
	return; // success
}

void CreateTexture::FromString(TCHAR* text, DXTEXTURE2D** pTex, ID3D11ShaderResourceView** pview, float *pRatio, int fontsize)
{
	HRESULT hr;
	
	/////////////////////////////////////////////////////////////////////////////
	// WinAPI������

	//�t�H���g�f�o�C�X����
	Init(fontsize);
	// �f�o�C�X�R���e�L�X�g�擾
	// �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
	HDC hdc = GetDC(NULL);
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	// �����f�[�^�擾
	TCHAR *c = text;
	// �������̎擾
	int textnum = wcslen(text);
	
	// �t�H���g�r�b�g�}�b�v�擾
	TEXTMETRIC TM;	// �t�H���g�̃��C�A�E�g���
	GetTextMetrics( hdc, &TM );	
	GLYPHMETRICS *GM = new GLYPHMETRICS[textnum];	// �����̃��C�A�E�g���
	BYTE **pBitmap = new BYTE*[textnum];
	CONST MAT2 Mat = {{0,1},{0,0},{0,0},{0,1}};

	// �e�����ɂ��ăr�b�g�}�b�v�擾
	FOR(textnum)
	{
		// unicode�̏ꍇ�A�����R�[�h�͒P���Ƀ��C�h������UINT�ϊ�
		UINT code = (UINT)c[i];
		// �t�H���g�r�b�g�}�b�v�擾
		// �w�肳�ꂽ�f�o�C�X�R���e�L�X�g�őI������Ă��� TrueType �t�H���g�����̃r�b�g�}�b�v�܂��̓A�E�g���C�����擾
		// �f�[�^��ptr�Ɋi�[
		DWORD size = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM[i], 0, NULL, &Mat);
		pBitmap[i] = new BYTE[size];
		GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM[i], size, pBitmap[i], &Mat);
	}

	// �f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���̊J��
	SelectObject(hdc, oldFont);
	ReleaseDC(NULL, hdc);

	/////////////////////////////////////////////////////////////////////////////
	// DirectX�e�N�X�`��������

	// �e�N�X�`���쐬
	D3D11_TEXTURE2D_DESC desc;
	UINT width = 0;
	FOR(textnum) width += GM[i].gmCellIncX;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = TM.tmHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// RGBA(255,255,255,255)�^�C�v
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;			// ���I�i�������݂��邽�߂̕K�{�����j
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// �V�F�[�_���\�[�X�Ƃ��Ďg��
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU����A�N�Z�X���ď�������OK

	// �e�N�X�`���쐬
	hr = m_pDevice->CreateTexture2D(&desc, 0, pTex);
	if (FAILED(hr))
	{
		FOR(textnum){ 
			SAFE_DELETEARRAY(pBitmap[i]); 
		}
		SAFE_DELETE(GM);
		SAFE_DELETE(pBitmap);

		ErrM.SetClassName(_T("CreateTexture::FromString::CreateTexture2D"));
		ErrM.SetErrorText(_T("�e�N�X�`���̍쐬�Ɏ��s"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
	// �V�F�[�_���\�[�X�r���[
	// ShaderResourceView���쐬����
	// ������쐬���Ă����Ȃ��ƃe�N�X�`�����\�[�X�Ƃ��Ďg�p�ł��Ȃ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;

	hr = m_pDevice->CreateShaderResourceView(*pTex, &srvDesc, pview);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("CreateTexture::FromString::CreateShaderResourceView"));
		ErrM.SetErrorText(_T("�e�N�X�`����ShaderResourceView�쐬�Ɏ��s"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}

	// �e�N�X�`�������b�N
	D3D11_MAPPED_SUBRESOURCE LockedRect;
	hr = m_pContext->Map(*pTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &LockedRect);


	/////////////////////////////////////////////////////////////////////////////
	// �e�N�X�`���ɏ������ݎ��s


	// �t�H���g���̏�������
	// iOfs_x, iOfs_y : �����o���ʒu(����)
	// iBmp_w, iBmp_h : �t�H���g�r�b�g�}�b�v�̕���
	// Level : ���l�̒i�K (GGO_GRAY4_BITMAP�Ȃ̂�17�i�K)
	int *iOfs_x = new int[textnum];
	int *iOfs_y = new int[textnum];
	int *iBmp_w = new int[textnum];
	int *iBmp_h = new int[textnum];

	FOR(textnum)
	{
		iOfs_x[i] = GM[i].gmptGlyphOrigin.x;
		iOfs_y[i] = TM.tmAscent - GM[i].gmptGlyphOrigin.y;
		iBmp_w[i] = GM[i].gmBlackBoxX + (4-(GM[i].gmBlackBoxX%4))%4;
		iBmp_h[i] = GM[i].gmBlackBoxY;
	}
	int Level = 17;
	int x, y;
	int Sum_x=0;
	DWORD Alpha, Color;
	ZeroMemory(LockedRect.pData, (width * TM.tmHeight)*sizeof(int32_t));
	
	// ��������
	for(int n = 0; n<textnum; ++n)
	{
		for(y=iOfs_y[n]; y<iOfs_y[n]+iBmp_h[n]; y++)
		{
			for(x=iOfs_x[n]; x<iOfs_x[n]+(int)GM[n].gmBlackBoxX; x++)
			{
				Alpha = (255 * pBitmap[n][x-iOfs_x[n] + iBmp_w[n]*(y-iOfs_y[n])]) / (Level-1);
				Color = 0x00ffffff | (Alpha<<24);
				memcpy((BYTE*)LockedRect.pData + LockedRect.RowPitch*y + 4 * (x + Sum_x), &Color, sizeof(DWORD));
			}
		}
		Sum_x += GM[n].gmCellIncX;
	}
	if (pRatio != NULL)*pRatio = (float)width / (float)TM.tmHeight;

	// �e�N�X�`���̃��b�N����
	m_pContext->Unmap(*pTex, 0);

	//////////////////////////////////////////////////
	// �I������
	
	FOR(textnum)
	{
		delete[] pBitmap[i];
	}
	SAFE_DELETE(GM);
	SAFE_DELETE(pBitmap);
	SAFE_DELETE(iOfs_x);
	SAFE_DELETE(iOfs_y);
	SAFE_DELETE(iBmp_w);
	SAFE_DELETE(iBmp_h);
	////////////////////////////////////////

	// success
}