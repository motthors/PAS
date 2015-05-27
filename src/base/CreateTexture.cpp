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
	//テクスチャ用フォント生成
	// フォントの生成
	LOGFONT lf = {
		fontsize,
		0,							// 文字セルまたは文字の高さ				
		0,							// 平均文字幅
		0,							// 文字送りの方向とX軸との角度
		0,							// ベースラインとX軸との角度
		0,							// フォントの太さ
		0,							// イタリック体指定
		0,							// 下線付き指定
		DEFAULT_CHARSET,			// キャラクタセット
		OUT_TT_ONLY_PRECIS,			// 出力精度
		CLIP_DEFAULT_PRECIS,		// クリッピングの精度
		CLEARTYPE_QUALITY,			// 出力品質
		DEFAULT_PITCH | FF_DONTCARE,// ピッチとファミリ
		_T("MS ゴシック")			// フォント名
	};

	if(!(hFont = CreateFontIndirect(&lf)))
	{
		ErrM.SetClassName(_T("CreateTexture::Init::pCreateTexture::CreateFontIndirect"));
		ErrM.SetErrorText(_T("フォントデバイス生成に失敗"));
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
	// WinAPI側準備

	//フォントデバイス生成
	Init(fontsize);
	// デバイスコンテキスト取得
	// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
	HDC hdc = GetDC(NULL);
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	// 文字データ取得
	TCHAR *c = text;
	// 文字数の取得
	int textnum = wcslen(text);
	
	// フォントビットマップ取得
	TEXTMETRIC TM;	// フォントのレイアウト情報
	GetTextMetrics( hdc, &TM );	
	GLYPHMETRICS *GM = new GLYPHMETRICS[textnum];	// 文字のレイアウト情報
	BYTE **pBitmap = new BYTE*[textnum];
	CONST MAT2 Mat = {{0,1},{0,0},{0,0},{0,1}};

	// 各文字についてビットマップ取得
	FOR(textnum)
	{
		// unicodeの場合、文字コードは単純にワイド文字のUINT変換
		UINT code = (UINT)c[i];
		// フォントビットマップ取得
		// 指定されたデバイスコンテキストで選択されている TrueType フォント文字のビットマップまたはアウトラインを取得
		// データはptrに格納
		DWORD size = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM[i], 0, NULL, &Mat);
		pBitmap[i] = new BYTE[size];
		GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM[i], size, pBitmap[i], &Mat);
	}

	// デバイスコンテキストとフォントハンドルの開放
	SelectObject(hdc, oldFont);
	ReleaseDC(NULL, hdc);

	/////////////////////////////////////////////////////////////////////////////
	// DirectXテクスチャ側準備

	// テクスチャ作成
	D3D11_TEXTURE2D_DESC desc;
	UINT width = 0;
	FOR(textnum) width += GM[i].gmCellIncX;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = TM.tmHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// RGBA(255,255,255,255)タイプ
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;			// 動的（書き込みするための必須条件）
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// シェーダリソースとして使う
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPUからアクセスして書き込みOK

	// テクスチャ作成
	hr = m_pDevice->CreateTexture2D(&desc, 0, pTex);
	if (FAILED(hr))
	{
		FOR(textnum){ 
			SAFE_DELETEARRAY(pBitmap[i]); 
		}
		SAFE_DELETE(GM);
		SAFE_DELETE(pBitmap);

		ErrM.SetClassName(_T("CreateTexture::FromString::CreateTexture2D"));
		ErrM.SetErrorText(_T("テクスチャの作成に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
	// シェーダリソースビュー
	// ShaderResourceViewを作成する
	// これを作成しておかないとテクスチャリソースとして使用できない
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
		ErrM.SetErrorText(_T("テクスチャのShaderResourceView作成に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}

	// テクスチャをロック
	D3D11_MAPPED_SUBRESOURCE LockedRect;
	hr = m_pContext->Map(*pTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &LockedRect);


	/////////////////////////////////////////////////////////////////////////////
	// テクスチャに書き込み実行


	// フォント情報の書き込み
	// iOfs_x, iOfs_y : 書き出し位置(左上)
	// iBmp_w, iBmp_h : フォントビットマップの幅高
	// Level : α値の段階 (GGO_GRAY4_BITMAPなので17段階)
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
	
	// 書き込み
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

	// テクスチャのロック解除
	m_pContext->Unmap(*pTex, 0);

	//////////////////////////////////////////////////
	// 終了処理
	
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