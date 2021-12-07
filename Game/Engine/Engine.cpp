#include "pch.h"
#include "Engine.h"

void Engine::Init(const WindowInfo& info)
{
	_window = info;
	ResizeWindow(info.width, info.height);

	// 그려질 화면 크기를 설정하는 부분
	_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);
				   // d3dx12.h에 포함이 되어 있는 부분. 

	_device = make_shared<Device>(); 
	_cmdQueue = make_shared<CommandQueue>(); 
	_swapChain = make_shared<SwapChain>(); 
	_rootSignature = make_shared<RootSignature>(); 
	_cb = make_shared<ConstantBuffer>(); 

	_device->Init();	// 에러나는 이유는 전방선언을 했지만 어떻게 구현되어 있는지 안알려줘서. 헤더 추가해주면 됨.  
	_cmdQueue->Init(_device->GetDevice(), _swapChain); 
	_swapChain->Init(info, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init(_device->GetDevice());		// 전역에서 꺼내 쓸 수 있긴 한데 만든 김에 Init에서 건내주는 방식으로 해줌. 
	_cb->Init(sizeof(Transform), 256);	// 어떤 사이즈로 만들어주냐가 고민인데 셰이더랑 맞춰주면 됨. float4
	// 일반적으로 물체마다 넘겨주는 정보는 대부분 위치정보완 관련된 정보를 넘겨줄때가 많아. 어떤 위치에 있고, 회전을 하고 transform 정보를 넘겨주는 경우가 많아서 임시적으로 Transform이라고 정했어. 
	// struct Transfrom이 크기가 크지 않지만 기본적으로 256배수가 되야 되니까 256바이트로 잡힐 것을 예상할 수 있고, 두번째 파라미터 숫자는 넉넉하게 256으로 잡아보도록 할거야. 
	// 사실 몇백가지 가는 경우가 바람직하진 않아. 그정도로 물체가 많이 그려야 되는 건 바람직하진 않고, 너무 늘어나게 되면 효율적이지 않게 동작할 수 있어. 
}

void Engine::Render()
{
	RenderBegin();

	// TODO : 나머지 물체들 그려준다

	RenderEnd();
}


void Engine::RenderBegin()
{
	_cmdQueue->RenderBegin(&_viewport, &_scissorRect);
}

void Engine::RenderEnd()
{
	_cmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int32 width, int32 height)	// 혹시 크기를 바꿀 수 있으니 인자를 받아주는 버전으로 만들어줌.
{
	_window.width = width; 
	_window.height = height; 

	RECT rect = { 0, 0, width, height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);	// window API임. 윈도우의 크기를 조절한 다음에 
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0);	// 윈도우 포지션을 내가 원하는 위치에 세팅을 해주겠다는 거. 
					// 윈도우 hwnd를 이용해서 윈도우 창을 width랑 height크기로 변경한 다음에 위치를 100, 100으로 해줘라는 명령 실행. 
	// ::은 글로벌 네임스페이스에서 함수를 찾아주겠다는 의미. 빼고 싶으면 빼도 되지만
	// :: 붙이면 장점은 일반적인 함수가 아니라 애당초 라이브러리에서 제공하는 윈도우즈 관련된 기능이라는 걸 암시하고 있다고 보면 됨. 
}
