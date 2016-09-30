#include "stdafx.h"
#include "TestVector.h"

std::ostream& operator<< (std::ostream& os, DirectX::FXMVECTOR xmv)
{
	DirectX::XMFLOAT2 vf2;
	DirectX::XMStoreFloat2(&vf2, xmv);
	os << "(" << vf2.x << ", " << vf2.y << ")";
	return os;
}

TestVector::TestVector()
{
}


TestVector::~TestVector()
{
}

void TestVector::exec()
{
	checkCosine3(DirectX::XMVectorSet(1, 1, 1, 0), DirectX::XMVectorSet(2, 3, 4, 0));
	checkCosine3(DirectX::XMVectorSet(1, 1, 0, 0), DirectX::XMVectorSet(-2, 2, 0, 0));
	checkCosine3(DirectX::XMVectorSet(-1, -1, -1, 0), DirectX::XMVectorSet(3, 1, 0, 0));
	checkCosine3(DirectX::XMVectorSet(-1, 3, 2, 0), DirectX::XMVectorSet(3, -4, 1, 0));
}

void TestVector::checkCosine3(DirectX::FXMVECTOR u, DirectX::FXMVECTOR v)
{
	float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(u, v));
	float len = DirectX::XMVectorGetX(DirectX::XMVectorMultiply(DirectX::XMVector3Length(u), DirectX::XMVector3Length(v)));
	float radian = acosf(dot / len);
	float degree = DirectX::XMConvertToDegrees(radian);
	std::cout << degree << " " << DirectX::XMConvertToDegrees(DirectX::XMVectorGetX(DirectX::XMVector3AngleBetweenVectors(u, v))) << std::endl;
}
