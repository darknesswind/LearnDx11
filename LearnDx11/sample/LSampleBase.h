#pragma once
class LD3DApplication;

class LSampleBase
{
public:
	LSampleBase(LD3DApplication* pApp);
	virtual ~LSampleBase();

	void create();
	virtual void draw();

protected:
	struct Vertex
	{
		Vector3 pos;
		DirectX::XMFLOAT4 clr;
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

	void fillBufDesc(D3D11_BUFFER_DESC& desc, UINT size, D3D11_BIND_FLAG bindFlag);

protected:
	LD3DApplication* m_pApp{ nullptr };
	ID3D11Device* m_pDevice{ nullptr };
	ID3D11DeviceContext* m_pContext{ nullptr };

	com_ptr<ID3D11Buffer> m_spVertexBuff;
	com_ptr<ID3D11Buffer> m_spIndexBuff;
	size_t m_vertexSize{ 0 };
	size_t m_indexSize{ 0 };

	D3D11_PRIMITIVE_TOPOLOGY m_topology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST };
	D3D11_FILL_MODE m_fillMode{ D3D11_FILL_SOLID };
	D3D11_CULL_MODE m_cullMode{ D3D11_CULL_BACK };
};

