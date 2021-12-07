#pragma once

class SwapChain;	// <>�ȿ� class ���� �ʹ� ������� �׳� ����� ���漱���� 
class DescriptorHeap; 

// gpu�� ���ָ� �� �ǵ� �������� device���� ��û�ϸ� �ƾ�. 
// ������ �ϰ��� �����ִ� ����� �����ϰ� �ٲ����. 

class CommandQueue
{
public:
	~CommandQueue();

	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);
	// �ʱ�ȭ �� �� device, swapChain, descriptorHeap�� �ѹ濡 �ǳ��� �� .
	void WaitSync();	// �潺�� �̿��� ��ٷ��ִ� �Լ�

	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();
	// ���������� ������ �׷����� �κа� ������ �־�. 

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr< ID3D12GraphicsCommandList> GetCmdList() { return _cmdList; }
private:
	// CommandQueue : DX12�� ����
	// ���ָ� ��û�� ��, �ϳ��� ��û�ϸ� ��ȿ����
	// [���� ���]�� �ϰ��� �������� ����ߴٰ� �� �濡 ��û�ϴ� ��
	ComPtr<ID3D12CommandQueue>			_cmdQueue;	// �ϰ��� �о� �ִٰ� ���߿� �ѹ濡 GPU���� �䱸�� �� �� Ȱ��
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;	// �ϰ� ��ü�� �־��ֱ� ���� �޸� ������ �Ҵ��ϴ� ����
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;	// �� �״�� ����Ʈ 
	// �� ���� 3���� �ٽ����� �κ�. 3�� ��Ʈ�� ��� �ϳ��� �뵵. ID3D12CommandQueue�� ��ǥ. 
	// �츮�� � ���ָ� ��û�� �� ���� �ϰ��� ���� �ϳ��� ���� �� ���� ������ ��ȿ�����̾�. 
	// ���� ����̶�� CommandQueue��� ������ �ϰ��� �ϳ��� ���� �״ٰ� �ѹ濡 ��� �ϰ����� �ѹ��� ��û�ϸ� ȿ����
	// �ù� ���� ������ �� �� �ϳ��� ������ ���ٴ� �ù踦 ��Ƽ� �����°� ȿ���� �̷� ����.
	// C#�� Ŀ�ǵ� ���ϰ� ����. �ϰ��� ���� �� ���� ó�� ���� �ʰ� Queue�� �־� ���ٰ� ó���ϴ� ����. 


	// Fence : ��Ÿ��(?)
	// CPU / GPU ����ȭ�� ���� ������ ����
	// �ϰ��� �������ٵ� ��쿡 ���� ������ �ϰ��� ������� �;� ������ �Ǵ� ��Ȳ�� ���� �� �־�. 
	// �׷� �� ��Ÿ���� ���� ������ ���ָ� �� �۾��� ���� �� ���� ��ٸ��ڴ� �� �� �潺��� �۾��� Ȱ���ؼ� ����Ѵ�. 
	// _fenceValue��°� �ִµ� CommandQueue�� ������ �ؼ� ����� �ϰ� ��. ���ݱ��� �ϰ��� ��Ƽ� ���ָ� ��µ� ���ָ� �ذŸ� 105���̶�� �ϸ�
	// 105���� �� ���� �� ���� ��ٸ��ڴ� �� �� fence�� �̿��Ѵ�. 
	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;

	shared_ptr<SwapChain>		_swapChain;
	// commandQueue�� �ϰ��� ���Ƽ� �����ϴ� �κ��̶� ���߿� ���� ���� ��(SwapChain, DescriptorHeap)�� �־��� ��ɵ鵵 �ʿ��ϱ� ������ �̷��� ��� ������ ����� �ذ�. ���� ���� ���ؼ� ���� ���°� 
	// �ڵ� ������ �� DescriptorHeap�� SwapChain�� �־� ����. 
};

