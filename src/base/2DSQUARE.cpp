#include"2DSQUARE.h"


D3D2DSQUARE::D3D2DSQUARE(DirectX11Base* pdx11, ShaderBox* psb, UINT Width, UINT Height)
{
	m_pDx11 = pdx11;
	m_pDevice = pdx11->GetDevice();
	m_pContext = pdx11->GetContext();
	m_pShaderBox = psb;

	this->Width = Width;
	this->Height = Height;

	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
}


D3D2DSQUARE::~D3D2DSQUARE()
{
	Invalidate();
}


void D3D2DSQUARE::Invalidate()
{
	RELEASE(m_pVertexBuffer);
	RELEASE(m_pIndexBuffer);
}


void D3D2DSQUARE::Init()
{
	HRESULT hr = -1;

	Invalidate();

	float per1TexelX = 1.0f / (float)Width;
	float per1TexelY = 1.0f / (float)Height;

	VERTEX vtx[4];
	// 頂点座標
	vtx[0].x = -1.0f;	vtx[0].y = 1.0f;	vtx[0].z = 0.0f;
	vtx[1].x = 1.0f;	vtx[1].y = 1.0f;	vtx[1].z = 0.0f;
	vtx[2].x = -1.0f;	vtx[2].y = -1.0f;	vtx[2].z = 0.0f;
	vtx[3].x = 1.0f;	vtx[3].y = -1.0f;	vtx[3].z = 0.0f;
	// テクセル
	vtx[0].u = 0.0f + 0.5f * per1TexelX; vtx[0].v = 0.0f + 0.5f * per1TexelY;
	vtx[1].u = 1.0f + 0.5f * per1TexelX; vtx[1].v = 0.0f + 0.5f * per1TexelY;
	vtx[2].u = 0.0f + 0.5f * per1TexelX; vtx[2].v = 1.0f + 0.5f * per1TexelY;
	vtx[3].u = 1.0f + 0.5f * per1TexelX; vtx[3].v = 1.0f + 0.5f * per1TexelY;

	// 頂点バッファを作成
	m_pDx11->CreateVertexBuffer(&m_pVertexBuffer, vtx, sizeof(vtx), 0);
	// インデックスバッファ作成
	USHORT Index[] = { 0, 1, 2, 3 };
	m_pDx11->CreateIndexBuffer(&m_pIndexBuffer, Index, sizeof(Index), 0);

	// 頂点シェーダーを作成する
	m_idVertexShader = m_pShaderBox->CreateVertexShader(_T("data/hlsl/VS_2DScreen.cso"));
}


void D3D2DSQUARE::SetVertexShader(UINT idpv)
{
	m_idVertexShader = idpv;
}

void D3D2DSQUARE::SetPixelShader(UINT idps)
{ 
	m_idPixelShader = idps;
}


	//２Ｄポリゴンをレンダリングする
void D3D2DSQUARE::Render()
{
	//pd3dDev->SetFVF( D3D2DFVF );
	//pd3dDev->SetStreamSource( 0, pd3d2DVertex, 0, sizeof(D3D2DVERTEX) );
	////▲ポリゴンを２枚使用
	//pd3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

	m_pShaderBox->SetShader(m_idVertexShader, m_idPixelShader);

	UINT	stride = sizeof(VERTEX);
	UINT	offset = 0;
	m_pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_pContext->DrawIndexed(4, 0, 0);
}
