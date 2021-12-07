#pragma once


// ��ȯ �罽
// [���� ����]
// - ���� ���� ���� �ִ� ��Ȳ�� ����
// - � �������� ��� ������� ������
// - GPU�� ������ ��� (����)
// - ����� �޾Ƽ� ȭ�鿡 �׷��ش�

// [���� �����]�� ��� ����?
// - � ����(Buffer)�� �׷��� �ǳ��޶�� ��Ź�غ���
// - Ư�� ���̸� ���� -> ó���� �ǳ��ְ� -> ������� �ش� ���̿� �޴´� OK
// - �츮 ȭ�鿡 Ư�� ����(���� �����) ������ش�

 

// [?]
// - �׷��� ȭ�鿡 ���� ����� ����ϴ� ���߿�, ���� ȭ�鵵 ���ָ� �ðܾ� ��
// - ���� ȭ�� ������� �̹� ȭ�� ��¿� �����
// - Ư�� ���̸� 2�� ����, �ϳ��� ���� ȭ���� �׷��ְ�, �ϳ��� ���� �ñ��...
// - Double Buffering!

// - [0] [1] ���̰� 2�� �ִٰ� ����
// ���� ȭ�� [0]  <-> GPU �۾��� [1] BackBuffer
// GPU�۾��� �Ϸ� �Ǿ����� [2]�� ���� ȭ�鿡 ����� ���ְ�, [1]�� GPU���� �ǳ��ְ� ���ָ� �ñ�� ��.
// GPU �۾��� ������ �ٽ� [1]�� ���� ȭ������ �����ְ�, [2]�� �ٽ� ���ָ� �ָ� ��. 
// �̷��� 2���� ���� �Դٰ��� �ϸ鼭 �ϳ��� ���� ȭ�� �׷��ְ�, �ϳ��� ���� �۾� �׷��޶� ��û�ϴ� �� double buffering�̶� ��. 
// swap chain�� �̷� �뵵��� ���� ��. 

class SwapChain
{
public:
	void Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
			
	void Present();
	void SwapIndex();

	ComPtr<IDXGISwapChain> GetSwapChain() { return _swapChain; }	// �׳� ����ִ� �κ�
	ComPtr<ID3D12Resource> GetRenderTarget(int32 index) { return _rtvBuffer[index]; }	// ���ҽ��� ��ȯ�ϴ� �κ�

	ComPtr<ID3D12Resource> GetBackRTVBuffer() { return _rtvBuffer[_backBufferIndex]; }		// 2���� ���� �߿��� ���� ����� ���ҽ��� �����. 
	// �׳� ���� �ε����� ���� �ͼ� GetRenderTarget ȣ���ؼ� ������ ��� ���� �� �ֱ� �ѵ� �ϴ� ���ΰ� ���߿� �ʿ� ������ ���� 

	D3D12_CPU_DESCRIPTOR_HANDLE GetBackRTV() { return _rtvHandle[_backBufferIndex];
}

private: 
	void CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void CreateRTV(ComPtr<ID3D12Device> device);

private:
	ComPtr<IDXGISwapChain>	_swapChain;

	ComPtr<ID3D12Resource>			_rtvBuffer[SWAP_CHAIN_BUFFER_COUNT];
	// 2��� ���ڸ� �Ź� �ϵ� �ڵ� �ϴ°� ������ �ϴϱ� EnginePch.h���� ����
	// ������ �׸���, Ÿ���� ����̴� �׸� ����̴� ��� ����� �� �� �־�. Ư�� ���̸� ���� Ÿ���̶�� �θ� �ž�.  

	// DescriptorHeap�� 3�ѻ� �������ذ�. 
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;
//	uint32							_rtvHeapSize = 0;	// _rtvHeap�� ���ؼ��� ���� �� �ִ� �����⿡ ����
	D3D12_CPU_DESCRIPTOR_HANDLE		_rtvHandle[SWAP_CHAIN_BUFFER_COUNT];	// ����Ʈ ����ó�� ���� ���ҽ� ��ü�� �����ϴ� ������ �ϴ°� �� view��� �� �� �־�. 

	uint32					_backBufferIndex = 0;
	// �� �״�� ���� backBuffer�� �����ΰ�? 
	// ���� ���۾��� �ϰ� �ִ¾ְ� �����ΰ� ������ ���ִ� ��. 0, 1�� �Դ� ���� �ϸ鼭 �ٲ�� �ȴ�. 
};

// �� �״�� ���۸� ������ ����� �డ���� �Դ� �����ϴ� �κ��� SwapChain�� �ٽ� �����̶�� �� �� �־�. 
