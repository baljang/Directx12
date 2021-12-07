#pragma once

// [����Ƽ¯]�� ���� ���� �������� �̷���� ��ü, ������ ����
class Mesh	// �� �������� � ������ �־�� ������ �츮�� ���Ǹ� ����� ��. 
{
public: 
	void Init(vector<Vertex>& vec);
	void Render();

	// ������ Ʈ������ ������ ����غ��ž�. 
	// �ܺο��� ������ �� �ְ�
	void SetTransform(const Transform& t) { _transform = t; }

private: 
	ComPtr<ID3D12Resource>		_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	_vertexBufferView = {};		
	// ������ ���� descriptorHeap�� �ƴ϶� D3D12_VERTEX_BUFFER_VIEW�� �Ǿ� �ִ� �� �� �� �־�. 
	// �⺻������ VIEW��� ���� ��ü�� �ٲ��� �ʾ�. ���� ���ҽ� ��ü�� GPU�޸𸮿� ���� �ɰǵ� �װ� ���縦 �ϰ� �ٸ� ������ �¸� ����� ���̴� �� �� 
	// �� ���ҽ� ��ü�� ���� Ȱ���ϴ°� �ƴ϶� ���� �� ��ǥ�� ���ҽ��� ��Ī�ϰ� ����Ű�� �����ϰ� �ִ� �ֶ�� ���� �ȴ�. 
	uint32 _vertexCount = 0;

	Transform _transform = {};
};

 