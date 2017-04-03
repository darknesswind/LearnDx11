#include "stdafx.h"

#include "ColorBoxSample.h"
#include "LD3DApplication.h"

ColorBoxSample::ColorBoxSample(LD3DApplication* pApp)
	: LSampleBase(pApp)
{
}


ColorBoxSample::~ColorBoxSample()
{
}

void ColorBoxSample::createModel(MeshData& mesh)
{
	mesh.vertices =
	{
		{ DirectX::XMFLOAT3(-1.f, -1.f, -1.f), DirectX::XMFLOAT4(DirectX::Colors::White) },
		{ DirectX::XMFLOAT3(-1.f, +1.f, -1.f), DirectX::XMFLOAT4(DirectX::Colors::Black) },
		{ DirectX::XMFLOAT3(+1.f, +1.f, -1.f), DirectX::XMFLOAT4(DirectX::Colors::Red) },
		{ DirectX::XMFLOAT3(+1.f, -1.f, -1.f), DirectX::XMFLOAT4(DirectX::Colors::Green) },
		{ DirectX::XMFLOAT3(-1.f, -1.f, +1.f), DirectX::XMFLOAT4(DirectX::Colors::Blue) },
		{ DirectX::XMFLOAT3(-1.f, +1.f, +1.f), DirectX::XMFLOAT4(DirectX::Colors::Yellow) },
		{ DirectX::XMFLOAT3(+1.f, +1.f, +1.f), DirectX::XMFLOAT4(DirectX::Colors::Cyan) },
		{ DirectX::XMFLOAT3(+1.f, -1.f, +1.f), DirectX::XMFLOAT4(DirectX::Colors::Magenta) },
	};

	mesh.indices =
	{
		0, 1, 2,
		0, 2, 3,
		4, 6, 5,
		4, 7, 6,
		4, 5, 1,
		4, 1, 0,
		3, 2, 6,
		3, 6, 7,
		1, 5, 6,
		1, 6, 2,
		4, 0, 3,
		4, 3, 7
	};
}
