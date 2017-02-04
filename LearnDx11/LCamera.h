#pragma once
class LCamera
{
public:
	LCamera();
	~LCamera();

	void setPosition(float radius, float phi, float theta);
	void XM_CALLCONV rotate(DirectX::FXMVECTOR RotationQuaternion);


	const Matrix& matrixf() const { return m_view; }
	DirectX::XMMATRIX matrix() const { return m_view; }
	DirectX::XMVECTOR position() const { return m_position; }
	DirectX::XMVECTOR target() const { return m_target; }
	DirectX::XMVECTOR up() const { return m_up; }
	DirectX::XMVECTOR lookat() const { return m_target - m_position; }

private:
	Matrix m_view;
	Vector3 m_position;
	Vector3 m_target;
	Vector3 m_up;
};

