#include "stdafx.h"
#include "TestMatrix.h"


TestMatrix::TestMatrix()
{
}


TestMatrix::~TestMatrix()
{
}

void TestMatrix::exec()
{


}

void TestMatrix::testDeterminant()
{
	DirectX::XMFLOAT3X3 m3 =
	{	 2, -5, 3,
		 1,  3, 4,
		-2,  3, 7,
	};


	DirectX::XMMATRIX m = DirectX::XMLoadFloat3x3(&m3);
	float det = DirectX::XMVectorGetX(DirectX::XMMatrixDeterminant(m));

	
}
