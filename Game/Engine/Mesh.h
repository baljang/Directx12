#pragma once

// [유니티짱]과 같이 뭔가 정점으로 이루어진 물체, 정점의 모임
class Mesh	// 그 정점에는 어떤 정보가 있어야 할지를 우리가 정의를 해줘야 해. 
{
public: 
	void Init(vector<Vertex>& vec);
	void Render();

	// 간단한 트랜스폼 정보를 사용해볼거야. 
	// 외부에서 세팅할 수 있게
	void SetTransform(const Transform& t) { _transform = t; }

private: 
	ComPtr<ID3D12Resource>		_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	_vertexBufferView = {};		
	// 예전에 만든 descriptorHeap이 아니라 D3D12_VERTEX_BUFFER_VIEW로 되어 있는 거 볼 수 있어. 
	// 기본적으로 VIEW라는 개념 자체는 바뀌지 않아. 실제 리소스 자체는 GPU메모리에 들어가게 될건데 그걸 묘사를 하고 다른 애한테 걔를 사용할 것이다 할 때 
	// 그 리소스 자체를 뭔가 활용하는게 아니라 뷰라는 걸 대표로 리소스를 지칭하고 가리키고 설명하고 있는 애라고 보면 된다. 
	uint32 _vertexCount = 0;

	Transform _transform = {};
};

 