#pragma once


// 교환 사슬
// [외주 과정]
// - 현재 게임 세상에 있는 상황을 묘사
// - 어떤 공식으로 어떻게 계산할지 던져줌
// - GPU가 열심히 계산 (외주)
// - 결과물 받아서 화면에 그려준다

// [외주 결과물]을 어디에 받지?
// - 어떤 종이(Buffer)에 그려서 건내달라고 부탁해보자
// - 특수 종이를 만들어서 -> 처음에 건내주고 -> 결과물을 해당 종이에 받는다 OK
// - 우리 화면에 특수 종이(외주 결과물) 출력해준다

 

// [?]
// - 그런데 화면에 현재 결과물 출력하는 와중에, 다음 화면도 외주를 맡겨야 함
// - 현재 화면 결과물은 이미 화면 출력에 사용중
// - 특수 종이를 2개 만들어서, 하나는 현재 화면을 그려주고, 하나는 외주 맡기고...
// - Double Buffering!

// - [0] [1] 종이가 2개 있다고 가정
// 현재 화면 [0]  <-> GPU 작업중 [1] BackBuffer
// GPU작업이 완료 되었으면 [2]를 현재 화면에 출력을 해주고, [1]을 GPU에게 건내주고 외주를 맡기면 됨.
// GPU 작업이 끝나면 다시 [1]을 현재 화면으로 돌려주고, [2]를 다시 외주를 주면 됨. 
// 이렇게 2개의 버퍼 왔다갔다 하면서 하나는 현재 화면 그려주고, 하나는 다음 작업 그려달라 요청하는 걸 double buffering이라 해. 
// swap chain이 이런 용도라고 보면 됨. 

class SwapChain
{
public:
	void Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
			
	void Present();
	void SwapIndex();

	ComPtr<IDXGISwapChain> GetSwapChain() { return _swapChain; }	// 그냥 뱉어주는 부분
	ComPtr<ID3D12Resource> GetRenderTarget(int32 index) { return _rtvBuffer[index]; }	// 리소스를 반환하는 부분

	ComPtr<ID3D12Resource> GetBackRTVBuffer() { return _rtvBuffer[_backBufferIndex]; }		// 2개의 버퍼 중에서 현재 백버퍼 리소스를 뱉어줘. 
	// 그냥 버퍼 인덱스를 꺼내 와서 GetRenderTarget 호출해서 꼭같은 기능 얻을 수 있긴 한데 일단 냅두고 나중에 필요 없으면 삭제 

	D3D12_CPU_DESCRIPTOR_HANDLE GetBackRTV() { return _rtvHandle[_backBufferIndex];
}

private: 
	void CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void CreateRTV(ComPtr<ID3D12Device> device);

private:
	ComPtr<IDXGISwapChain>	_swapChain;

	ComPtr<ID3D12Resource>			_rtvBuffer[SWAP_CHAIN_BUFFER_COUNT];
	// 2라는 숫자를 매번 하드 코딩 하는게 지저분 하니까 EnginePch.h에서 관리
	// 렌더는 그리다, 타겟은 대상이니 그릴 대상이다 라고 요약을 할 수 있어. 특수 종이를 렌더 타겟이라고 부를 거야.  

	// DescriptorHeap의 3총사 복붙해준거. 
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;
//	uint32							_rtvHeapSize = 0;	// _rtvHeap을 통해서도 얻을 수 있는 정보기에 삭제
	D3D12_CPU_DESCRIPTOR_HANDLE		_rtvHandle[SWAP_CHAIN_BUFFER_COUNT];	// 아파트 문서처럼 실제 리소스 자체를 설명하는 역할을 하는게 이 view라고 할 수 있어. 

	uint32					_backBufferIndex = 0;
	// 말 그대로 현재 backBuffer는 누구인가? 
	// 현재 뒷작업을 하고 있는애가 누구인가 추적을 해주는 애. 0, 1을 왔다 갔다 하면서 바뀌게 된다. 
};

// 말 그대로 버퍼를 여러개 만들어 줘가지고 왔다 갔다하는 부분이 SwapChain의 핵심 역할이라고 볼 수 있어. 
