#include "pch.h"
#include "SwapChain.h"
//#include "Engine.h"	// Engine을 들여다 놓으면 Device, CommandQueue, SwapChain 3총사 다 활용할 확률 높은데 필요한 거 한땀 한땀 추가하는 거 피곤할 수 있어. 
//#include "Device.h"

void SwapChain::Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	CreateSwapChain(info, dxgi, cmdQueue);
	CreateRTV(device); 
}

void SwapChain::Present()	// 현재 화면에 어떤 애가 출력되어야 하는지 지정 되면 그려줘 하는 부분
{
	// Present the frame.
	_swapChain->Present(0, 0);	
}

void SwapChain::SwapIndex()		// 말 그대로 index를 스왑한다 0이면 1, 1이면 0
{
	_backBufferIndex = (_backBufferIndex + 1) % SWAP_CHAIN_BUFFER_COUNT;
}

void SwapChain::CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	// 이전에 만든 정보 날린다
	_swapChain.Reset();	// Init 두번 했을 까봐 기존거 날리는건데 굳이 없어도 되긴 함. 

	DXGI_SWAP_CHAIN_DESC sd;	// swapchain에 대한 묘사를 해주고 있는 거. 어떻게 만들 것이다.
	// 종이도 화면에 그리는 그 그림을 그려주는 것이다 보니까 화면 크기랑 똑같이 만들어주는 걸 볼 수 있어. 
	// 종이를 어떻게 만들어 줄 것인지 여기서 결정해 주는 거.ㅣ 
	sd.BufferDesc.Width = static_cast<uint32>(info.width); // 버퍼의 해상도 너비
	sd.BufferDesc.Height = static_cast<uint32>(info.height); // 버퍼의 해상도 높이
	sd.BufferDesc.RefreshRate.Numerator = 60; // 화면 갱신 비율
	sd.BufferDesc.RefreshRate.Denominator = 1; // 화면 갱신 비율
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 버퍼의 디스플레이 형식, 8비트씩 활용을 하고 있다. 최종적으로 32비트 활용하는 거 예상할 수 있어. 
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1; // 멀티 샘플링 OFF <- 나중에 다루게 될거야. 
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 후면 버퍼에 렌더링할 것 
	sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT; // 전면+후면 버퍼, 아까 2라고 설정했더 것
	sd.OutputWindow = info.hwnd;
	sd.Windowed = info.windowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // 전면 후면 버퍼 교체 시 이전 프레임 정보 버림
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	dxgi->CreateSwapChain(cmdQueue.Get(), &sd, &_swapChain);
	// commandQueue이런 애들은 device를 통해 만들었는데 dxgi를 통해서 만들어준 걸 알 수 있어. 
	// dxgi가 출력과 관련된 부분들이 들어가 있었는데 swapchain 같은 경우에도 그래픽스를 활용할 때만 사용하는 개념이 아니다 보니 여기에 들어가 있어. 
	// 만들어준 정보를 _swapCahin에 넣어 달라고 요청을 해준거. 

	for (int32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		_swapChain->GetBuffer(i, IID_PPV_ARGS(&_rtvBuffer[i]));
	// swapChain도 만들어주겠지만 우리가 요청한 버퍼도 같이 만들어주게 될 거야. 
	// GetBuffer를 이용해서 _renderTargets라는 아이에게 다시 저장을 해주는 것을 볼 수 있어. 
	// swapChain을 만들어 주고 간접적으로 버퍼도 만들어 줬으니까 그 버퍼를 다시 꺼내서 자주 사용할 거니까 _renderTargets이라는 부분에 저장을 하고 있는거. 
	// &_renderTargets[i]를 _rtvBuffer로 이름을 바꿔줬음. 

}


// DescriptorHeap에 있던 코드
// .cpp에서는 <>안에 class를 넣어 전방선언을 해도 의미가 없어. 
void SwapChain::CreateRTV(ComPtr<ID3D12Device> device)
{
	// Descriptor (DX12) = View (~DX11)
	// [서술자 힙]으로 RTV 생성
	// DX11의 RTV(RenderTargetView), DSV(DepthStencilView), 
	// CBV(ConstantBufferView), SRV(ShaderResourceView), UAV(UnorderedAccessView)를 전부!

	// 예전에는 각기 리소스마다 각각의 뷰가 있었어. 다 따로관리하고 있었는데 DX12부터는 DescriptorHeap을 이용해서 모든 애들을 다 묘사하고 관리하게 된다는 차이점 생김. 
	// 그리고 결국에는 우리가 리소스 자체를 swapChain에서 관리 했었어. _renderTargets라는 애가 우리의 리소스라고 볼 수 있어. 
	// 그것을 묘사하는 기안서를 하나 더 만들고 있다고 보면 됨. 쓸데 없는 일 같긴 한데 그렇게 규격이 정해져 있음. 

	int32 rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);	// 내부에 _rtvHeapSize안들고 있기로 했으니 int32로 스택 메모리에 잠시 활용하고 날려주기로
	// RenderTargetView의 사이즈를 하나 계산하고 있어. 

	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// 타입은 RTV다 넣어주고 있는 거
	rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;	// 몇개를 만들어 줄것이냐? 2개를 만들어 주세요. 
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvDesc.NodeMask = 0;
	// 이걸 정보를 채워서 CreateDescriptorHeap을 때리게 되면 RTV 배열이 만들어지게 되면셔 2개짜리 목록을 담을 수 있는 크기의 배열이 만들어짐을 알 수 있어. 
	// _rtvHeap은 배열같은 느낌이라고 볼 수 있어. 

	// descriptHeap은 일종의 배열이라고 볼 수 있어. 배열은 배열인데 같은 애들끼리만 같이 뭉뚱그려 저장할 수 있는 그런 배열이라 할 수 있어. 
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
//		_rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * _rtvHeapSize);
		_rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * rtvHeapSize);
		// CD가 들어간 건 추가해줬던 d3dx12.h 라이브러리에 있는 헬퍼 클래스들 이라고 했어. 
		// 들어가서 보면 결국 handle.ptr = base.ptr + offsetScaledByIncrementSize; 이걸 해주는 거
		// handle이 첫 시작 주소를 나타내고 있다고 보면 됨. rtvHeapBegin.ptr = rtvHeapBegin.ptr + offsetScaledByIncrementSize; 이런 느낌
		// _rtvHandle[i].ptr = rtvHeapBegin.ptr + i * _rtvHeapSize; 이렇게 바꿔치기 해주는 거랑 마찬가지
		// 시작 주소에서 i에다가 Heap사이즈를 곱해주겠다는 거니까 첫번째 데이터는 0이니까 시작 주소가 되는게 맞고 그 다음에는 i가 1일 테니까 HeapSize만큼 더해줘서 
		// 다음 칸으로 넘어가서 그 정보를 채워주겠다 주장을 하는 거. 
		// Handle이라는 용어가 자주 사용되는데 Handle은 일반적으로 포인터처럼 원격적으로 접근해서 사용을 하는 느낌이라고 이해해주면 됨. 
		// windows api에서 창에 대한 핸들 이런 용어 사용해. 정수긴 정수인데 어떤 객체를 가리키는 객체라고 보면 됨. 
		// cpu에서는 외주를 맡긴사람 입장에서는 핸들을 통해서 어떤 애인지 알려줄 수 있는 거고, GPU에서도 찰떡같이 알아듣는 그런 상황이 발생한거. 

//		device->CreateRenderTargetView(swapChain->GetRenderTarget(i).Get(), nullptr, _rtvHandle[i]);
		device->CreateRenderTargetView(_rtvBuffer[i].Get(), nullptr, _rtvHandle[i]);	// DescriptorHeap코드를 SwapChain에 옮긴 것이므로 SwapChain의 _rtvBuffer를 바로 쓸 수 있게 됨. 
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