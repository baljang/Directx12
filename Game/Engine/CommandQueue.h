#pragma once

class SwapChain;	// <>안에 class 쓰면 너무 길어져서 그냥 여기다 전방선언함 
class DescriptorHeap; 

// gpu에 외주를 줄 건데 옛날에는 device에서 요청하면 됐어. 
// 이제는 일감을 던저주는 방식이 복잡하게 바뀌었어. 

class CommandQueue
{
public:
	~CommandQueue();

	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);
	// 초기화 할 때 device, swapChain, descriptorHeap을 한방에 건내준 거 .
	void WaitSync();	// 펜스를 이용해 기다려주는 함수

	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();
	// 실직적으로 게임이 그려지는 부분과 관련이 있어. 

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr< ID3D12GraphicsCommandList> GetCmdList() { return _cmdList; }
private:
	// CommandQueue : DX12에 등장
	// 외주를 요청할 때, 하나씩 요청하면 비효율적
	// [외주 목록]에 일감을 차곡차곡 기록했다가 한 방에 요청하는 것
	ComPtr<ID3D12CommandQueue>			_cmdQueue;	// 일감을 밀어 넣다가 나중에 한방에 GPU한테 요구를 할 때 활용
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;	// 일감 자체를 넣어주기 위한 메모리 공간을 할당하는 역할
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;	// 말 그대로 리스트 
	// 이 위에 3개가 핵심적인 부분. 3종 세트가 사실 하나의 용도. ID3D12CommandQueue가 대표. 
	// 우리가 어떤 외주를 요청할 때 외주 하고픈 일이 하나씩 있을 때 마다 던지면 비효율적이야. 
	// 외주 목록이라는 CommandQueue라는 애한테 일감을 하나씩 차곡 쌓다가 한방에 모든 일감들을 한번에 요청하면 효율적
	// 택배 많이 보내야 할 때 하나씩 보내기 보다는 택배를 모아서 보내는게 효율적 이런 느낌.
	// C#의 커맨드 패턴과 유사. 일간이 생길 때 당장 처리 하지 않고 Queue에 넣어 놨다가 처리하는 패턴. 


	// Fence : 울타리(?)
	// CPU / GPU 동기화를 위한 간단한 도구
	// 일감을 던져줄텐데 경우에 따라 던져준 일감의 결과물이 와야 진행이 되는 상황이 있을 수 있어. 
	// 그럴 때 울타리를 만들어서 상대방이 외주를 준 작업이 끝날 때 까지 기다리겠다 할 때 펜스라는 작업을 활용해서 사용한다. 
	// _fenceValue라는게 있는데 CommandQueue랑 연동을 해서 사용을 하게 됨. 지금까지 일감을 모아서 외주를 줬는데 외주를 준거를 105번이라고 하면
	// 105번이 다 끝날 때 까지 기다리겠다 할 때 fence를 이용한다. 
	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;

	shared_ptr<SwapChain>		_swapChain;
	// commandQueue는 일감들 몰아서 실행하는 부분이라 나중에 가면 여기 둘(SwapChain, DescriptorHeap)에 넣어준 기능들도 필요하기 때문에 이렇게 멤버 변수로 만들어 준거. 전방 선언 안해서 에러 나는거 
	// 코드 수정할 때 DescriptorHeap은 SwapChain에 넣어 줬음. 
};

