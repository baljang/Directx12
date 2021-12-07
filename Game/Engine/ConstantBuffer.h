#pragma once

// ���� ������ Buffer �� ������ִ� ����
// �׷��� ���� ���� ������ ����ϰ� �ִ����� ������ �ذ����� �������� �����͸� �о�־��ָ� ���� ����ϰ� �ִ�
// ������ �ǵ帮�� �ʰ� ���� ������ �����͸� �о� �־��ְ�, �� �����͸� ���� ������ �ϴ� ������ ������ �Ѵ�. 

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(uint32 size, uint32 count);

	void Clear();
	void PushData(int32 rootParamIndex, void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);

private:
	void CreateBuffer();


private: 
	ComPtr<ID3D12Resource>	_cbvBuffer;		// ComPtr�� ������ �ظ��ϸ� GPU�� �����Ͷ�� �� �� �־�. �긦 ������ָ� Buffer�� �������. 
	BYTE*					_mappedBuffer = nullptr;	// �츮�� CPU�ʿ��� �����͸� �о���� �� ����ϴ� ����, �Ѳ��� �� �� ����ϴ� �κ�
	// Mesh���� ���캻 memcpy�� �ϱ� ���� ���� ����

	uint32					_elementSize = 0;	// �� ����� ũ��� 
	uint32					_elementCount = 0;	// �� ����� ����
	// �� ���ۿ� ũ�Ⱑ ���� �Ű� �������� ������ �ϴµ� ī��Ʈ�� �ϳ��� ������ ũ�⸦ ����ϴ°�. 
	// ���۴� ���� �ϳ��� �̾߱� �ϴ°� �ƴ϶� �� ��� �Ķ��� ������ ������ ��ü�� cbv ���۶�� �̾߱� �ϰ� �ִٰ� �� �� �־�. 

	uint32					_currentIndex = 0;
	// ������ ����ߴ��� �����ϰ� �ִ� ��. 0���� �����ؼ� �þ�� �ǰ� �������� �� �������� �׷����� �ٽ� 0���� �ʱ�ȭ�ؼ� ���� �������� �׸� �� �ְ� ��. 
	// �ϳ��� �׸��ٰ� ��� �׸��� �׷������� �ٽ� 0������ �� ���� �����͸� �޾��ִ� ��. 

	// �̷��� �ʿ��� �غ񹰵��� ������ ����. 
};

