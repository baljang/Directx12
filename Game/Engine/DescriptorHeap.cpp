#include "pch.h"
#include "DescriptorHeap.h"
#include "SwapChain.h"

// .cpp에서는 <>안에 class를 넣어 전방선언을 해도 의미가 없어. 
void DescriptorHeap::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain;	// 나중에 활용할 거니까 스마트 포인터 저장하고 있어. 

	// Descriptor (DX12) = View (~DX11)
	// [서술자 힙]으로 RTV 생성
	// DX11의 RTV(RenderTargetView), DSV(DepthStencilView), 
	// CBV(ConstantBufferView), SRV(ShaderResourceView), UAV(UnorderedAccessView)를 전부!

	// 예전에는 각기 리소스마다 각각의 뷰가 있었어. 다 따로관리하고 있었는데 DX12부터는 DescriptorHeap을 이용해서 모든 애들을 다 묘사하고 관리하게 된다는 차이점 생김. 
	// 그리고 결국에는 우리가 리소스 자체를 swapChain에서 관리 했었어. _renderTargets라는 애가 우리의 리소스라고 볼 수 있어. 
	// 그것을 묘사하는 기안서를 하나 더 만들고 있다고 보면 됨. 쓸데 없는 일 같긴 한데 그렇게 규격이 정해져 있음. 

	_rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	// RenderTargetView의 사이즈를 하나 계산하고 있어. 
	
	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// 타입은 RTV다 넣어주고 있는 거
	rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;	// 몇개를 만들어 줄것이냐? 2개를 만들어 주세요. 
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvDesc.NodeMask = 0;
	// 이걸 정보를 채워서 CreateDescriptorHeap을 때리게 되면 RTV 배열이 만들어지게 되면셔 2개짜리 목록을 담을 수 있는 크기의 배열이 만들어짐을 알 수 있어. 
	// _rtvHeap은 배열같은 느낌이라고 볼 수 있어. 

	// descriptHeap은 일종의 배열이라고 볼 수 있어. 배연은 배열인데 같은 애들끼리만 같이 뭉뚱그려 저장할 수 있는 그런 배열이라 할 수 있어. 
	// 같은 종류의 데이터끼리 배열로 관리
	// RTV 목록 : [ ] [ ] 
	device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&_rtvHeap));

	// 배열 안의 내용물을 채워주기 위해 아래 코드가 실행된다. 
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = _rtvHeap->GetCPUDescriptorHandleForHeapStart();
	// 주소를 꺼내주게 될거야. 
	// D3D12_CPU_DESCRIPTOR_HANDLE에 들어가서 살펴보면 SIZE_T ptr; 이라는 어떤 주소값을 갖고 있는 거 볼 수 있어. 
	// 다만 포인터는 아니고 어떤 정수 형태로 주소를 관리하고 있는 걸 알 수 있어. 
	// 얘가 RTV 목록의 시작 주소를 가리키고 있다고 볼 수 있어. 

	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)	// 2번 loop
	{
		_rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * _rtvHeapSize);
		// CD가 들어간 건 추가해줬던 d3dx12.h 라이브러리에 있는 헬퍼 클래스들 이라고 했어. 
		// 들어가서 보면 결국 handle.ptr = base.ptr + offsetScaledByIncrementSize; 이걸 해주는 거
		// handle이 첫 시작 주소를 나타내고 있다고 보면 됨. rtvHeapBegin.ptr = rtvHeapBegin.ptr + offsetScaledByIncrementSize; 이런 느낌
		// _rtvHandle[i].ptr = rtvHeapBegin.ptr + i * _rtvHeapSize; 이렇게 바꿔치기 해주는 거랑 마찬가지
		// 시작 주소에서 i에다가 Heap사이즈를 곱해주겠다는 거니까 첫번째 데이터는 0이니까 시작 주소가 되는게 맞고 그 다음에는 i가 1일 테니까 HeapSize만큼 더해줘서 
		// 다음 칸으로 넘어가서 그 정보를 채워주겠다 주장을 하는 거. 
		// Handle이라는 용어가 자주 사용되는데 Handle은 일반적으로 포인터처럼 원격적으로 접근해서 사용을 하는 느낌이라고 이해해주면 됨. 
		// windows api에서 창에 대한 핸들 이런 용어 사용해. 정수긴 정수인데 어떤 객체를 가리키는 객체라고 보면 됨. 
		// cpu에서는 외주를 맡긴사람 입장에서는 핸들을 통해서 어떤 애인지 알려줄 수 있는 거고, GPU에서도 찰떡같이 알아듣는 그런 상황이 발생한거. 

		device->CreateRenderTargetView(swapChain->GetRenderTarget(i).Get(), nullptr, _rtvHandle[i]);
	}
	// 각각에 대해서 진짜 데이터가 누구누구 였는지를 참조해서 view를 만들어 준다고 볼 수 있어. 
	// 렌더 타겟을 어디에 만들어 놨는지 생각을 해보면 SwapChain 쪽에 넣어 놨었어. 
	// ComPtr<ID3D12Resource>	_renderTargets[SWAP_CHAIN_BUFFER_COUNT]; 얘를 묘사하고 있는 거 
	// 0번, 1번 RenderTarget을 꺼내와서 CreateRenderTargetView로 RTV를 만들어 주는 거. 

	// 이렇게 우여곡절 끝에 CreateRenderTargetView를 해준 거
	// _rtvHandle[i]주소에 접근을 해서 그 정보를 swapChain->GetRenderTarget(i).Get() 이 데이터로 채워줘라 라고 하는 거. 

	// 여기까지 끝내게 되면 결국 GPU에게 view라는 걸 넘겨줄 수 있게끔 준비가 된 상태가 되었다고 보면 됨. 
	// 기안서 같은 느낌. 현재 상황이 무엇무엇이다 라는걸 이 view를 통해 전달을 해주면 되는 거. 
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetBackBufferView()
{
	return GetRTV(_swapChain->GetCurrentBackBufferIndex());
// 현재 _swapChain에서 CurrentBackBufferIndex를 참고해가지고 그거의 RTV를 꺼내서 반환을 해주고 있어. 
}