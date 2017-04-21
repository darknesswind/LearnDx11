#pragma once
class LD3DApplication;

class LSampleBase
{
public:
	LSampleBase(LD3DApplication* pApp);
	virtual ~LSampleBase();

	void create();
	virtual void update();
	virtual void draw();

protected:
	struct Vertex
	{
		Vector3 pos;
		Vector3 target;
		Vector3 normal;
		Vector2 tex0;
		Vector2 tex1;
		DxColor clr;

		Vertex() = default;
		Vertex(const Vector3& _pos, const DxColor& _clr) : pos(_pos), clr(_clr) {}
	};
	typedef std::vector<Vertex> Vertices;
	typedef std::vector<size_t> Indices;
	struct MeshData	
	{
		Vertices vertices;
		Indices indices;
	};


	virtual void createInputLayout();
	virtual void createModel(MeshData& mesh);

protected:
	LD3DApplication* m_pApp{ nullptr };
	ID3D11Device* m_pDevice{ nullptr };
	ID3D11DeviceContext* m_pContext{ nullptr };

	com_ptr<ID3D11Buffer> m_spVertexBuff;
	com_ptr<ID3D11Buffer> m_spIndexBuff;
	size_t m_vertexSize{ 0 };
	size_t m_indexSize{ 0 };

	ID3DX11EffectMatrixVariable* m_gWorldViewProj;
	ID3DX11EffectVariable* m_gTime;

	D3D11_PRIMITIVE_TOPOLOGY m_topology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST };
	D3D11_FILL_MODE m_fillMode{ D3D11_FILL_SOLID };
	D3D11_CULL_MODE m_cullMode{ D3D11_CULL_BACK };

	D3D11_BUFFER_DESC m_vertexDesc{ 0 };
	D3D11_BUFFER_DESC m_indexDesc{ 0 };

};

