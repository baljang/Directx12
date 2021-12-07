#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Engine.h"

CommandQueue::~CommandQueue()
{
	::CloseHandle(_fenceEvent);	// 이벤트를 정리하는 부분
	// 하지 않아도 프로그램에 문제가 있지는 않지만 동적할당 후 delete 하는 것이 좋은 습관인것처럼 
	// event 활용 후에는 신호등 꺼주는 뒤처리를 해주는게 정석적이기에 추가한 것. 
}

void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	// device를 통해 뭔가 작업하고 있는 걸 주의깊게 보면 됨. 
	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue));
							// (어떤식으로 만들지, 어디다 만들지)
	// cmdQueue를 new로 만드는게 아니라 COM 객체의 특정 함수를 통해 간접적으로 만들어주고 있어. 
	// 3총사를 다 이런 식으로 만들어주었어. 

	// - D3D12_COMMAND_LIST_TYPE_DIRECT : GPU가 직접 실행하는 명령 목록
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));

	// GPU가 하나인 시스템에서는 0으로
	// DIRECT or BUNDLE
	// Allocator
	// 초기 상태 (그리기 명령은 nullptr 지정)
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));
	// 위에서 만든 allocator 넘겨주고 있어. std::vector와 유사해. vector에서 어떤 할당자를 이용해 메모리를 할당하다가 vector에 clear를 때리면 
	// 그 메모리를 싸그리 날리는게 아니라 capacity는 그대로인데 size만 resize가 되어 줄어들어. commandQueue가 그런 식으로 동작하고 있어.
	// 나중에 commandlist에 있는 내용 날리고 밀어 넣을 텐데 메모리 날리는게 아니라 allocator가 메모리 관리하고 있는 느낌	  

	// CommandList는 Close / Open 상태가 있는데
	// Open 상태에서 Command를 넣다가 Close한 다음 제출하는 개념
	_cmdList->Close();

	// 새부적으로 뭘 하는지는 나중에 다시 볼 때 찾아보고 일단 넘어가기. 

	// CreateFence
	// - CPU와 GPU의 동기화 수단으로 쓰인다
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));	// 울타리 만들어주고 있어. 
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	// CreateEvent 자체는 딱히 directX 특유기능은 아니고 일반적으로 멀티 스레드에서 동기화 할 때도 자주 쓰이는 방법이야. 
	// 신호등 같은 개념. 빨간불일 때는 멈췄다가 파란불이 되면 건너가는 것과 마찬가지로 Event를 만들어주고 신호 꺼주고 특정 조건 만족하면 파란불이 켜지게끔 요청
	// 파란 불이 켜질 때 까지 기다리는 상태로 동기화를 해주게 됨. 그런 용도로 활용. 
}

void CommandQueue::WaitSync()
{
	// Advance the fence value to mark commands up to this fence point.
	_fenceValue++;	// 펜스에 어떤 값이 있어. 0부터 1씩 증가해 가는 거. 어떤 일감을 다 요청을 시킨 다음에 번호를 매길거야. 
	// 상태에 대해 번호를 매긴 다음에 

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	_cmdQueue->Signal(_fence.Get(), _fenceValue);
	// commandQueue에도 그 번호를 건내주고 

	// Wait until the GPU has completed commands up to this fence point.
	if (_fence->GetCompletedValue() < _fenceValue)
	{
		// Fire event when GPU hits current fence.  
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);
		// 그 번호까지 만약에 끝났으면 fenceEvent를 실행시켜 주라 하고 있는 거. 
		// 이 부분까지는 GPU가 실행하다가 여기서 건내준 숫자에 해당하는 일감까지 끝냈다 싶으면
		// event를 틀어서 파란불을 켜주게 될거고 

		// Wait until the GPU hits current fence event is fired.
		::WaitForSingleObject(_fenceEvent, INFINITE);
		// CPU가 살찍 기다리게 됨. 
		// 기다리다가 파란불 되면 진행하게 되는거. 

		// 기본적으로 어떤 값을 줘서 해당 부분을 끝날 때 까지 기다리려주고 있는 부분
		// CPU가 GPU가 일이 끝날 때 까지 기다리는 방식이다 보니까 CPU가 어떤 식으로든 놀고 있으면 좋은 코드라고 볼 수 없어. 
		// 간단하게 만들기 위해 이렇게 만든거야. 효율적 코드는 아님. 
	}
}


void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	_cmdAlloc->Reset();	
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);
	// std::vector기준으로 clear 해준 거랑 유사. 깔끔하게 날려준거. 메모리가 줄어들진 않고 capacity는 그대로인 느낌

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT, // 화면 출력
		D3D12_RESOURCE_STATE_RENDER_TARGET); // 외주 결과물
	// CD..는 헬퍼 클래스라고 했는데 f12로 들어가 보면 이런 저런 기능들을 채워주는 걸 알 수 있어. 
	// 세부적으로 무엇을 하는지 분석할 필요는 없고 넣어주고 있는 부분이 핵심이야. Transition함수에서도 많은 거 받고 있지만 결국에는
	// 3가지가 가장 핵심적인 정보야. 하고 있는건 1. 어떤 리소스를 받아주고 있는 거 볼 수 있어. SwapChain이랑 맞물려서 동작을 하고 있는 건데 
	// 0번과 1번을 왔다갖다 바꿔치기 하면서 그 부분을 설정하고 있다고 보면 됨. 2. PRESENT는 화면 출력 3. RENDER_TARGET이라는 건 외주 결과물 받아주는 
	// 뒤에서 작업을 할 때 상태들을 말하는데 Transition에 커서 대면 PRESENT가 before, RENDER_TARGET이 after라는 걸 볼 수 있어. 
	// 외주 결과물로 바꿔치기 하겠다는게 이 부분이라고 할 수 있어. 현재 백버퍼 리소스를 이동 시켜서 GPU 작업 용도로 활용하겠다고 요청을 만들어주는 부분
	// 바로 실행 되는게 아니라 barrior 형태로 만들어지는데 그걸 _cmdList->ResourceBarrier(1, &barrier);로 밀어 넣었어.


	// 어떤 정책으로 어떤 건물을 임대를 하겠다고 서명을 해줄거야. 
	_cmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get()); // 에러나면 #include "Engine.h"해줘.
	// 이 서명을 활용을 하겠다 선포한거. 

	GEngine->GetCB()->Clear(); // 컨스턴트 버퍼 클리어. 

	_cmdList->ResourceBarrier(1, &barrier);	

	// Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
	// 커멘드 리스트를 밀었으면 다시 세팅을 해야 한다고 하니까 
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);

	// Specify the buffers we are going to render to.
	// 어떤 버퍼에다 그림을 그려야 하는지를 다시 언급을 해줘야 하는데 여기서 해줌
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView =  _swapChain->GetBackRTV();	// 백버퍼를 꺼내온다음에 거기에 대상으로 GPU한테 일감을 그려달라고 요청하는거
	_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);	
	// LightSteelBlue라고 설정을 해줬는데 아무것도 없는 상태에서 켜보게 되면 연한 하늘색이 뜨게 됨. 색깔이 느는 걸 보고 코드가 실행되고 있는지 테스트를 해보게 될거야. 
	_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, nullptr);	// Ouput Merger
	// GPU가 작업할 걸 백버퍼라고 부르는데 백버퍼를 GPU한테 알려주는 부분이 여기에 들어갔어. 
	// 바꿔치기로 왔다갔다 하는 부분을 작업하고 있는 거고, 원래 화면을 그리고 있는 용도에서 활용하고 있던 애를 뒤에 있던 백버퍼로 바꿔치기 한다음에 GPU한테 이제 얘한테 결과물을 그려줘라고 요청하는 일련의 과정들
	// GPU한테 외주를 항상 백버퍼에 해달라고 요청을 한 다음에 나중에 가면 이런저런 물체들을 GPU에게 요청해서 추가적으로 일감을 요청해줄거고
	// 마지막으로 일감을 마무리 지을 때는 RenderEnd를 호출해줄건데
}

void CommandQueue::RenderEnd()	 
{
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, // 외주 결과물
		D3D12_RESOURCE_STATE_PRESENT); // 화면 출력
	// 유사한 형태로 동작을 하는데 순서가 바뀌어 있어. 
	// 기존에는 RENDER_TARGET 뒤에서 GPU작업 용도로 활용 되던애를 화면출력 용도로 다시 꺼꾸로 바꿔치기 해주는 상황이되는 거. 
	// 뒤에 있던 애를 다시 앞단으로 옮기고 있는 상황이 되는 거. 

	_cmdList->ResourceBarrier(1, &barrier);		// 일감 밀어 넣어 주는거
	_cmdList->Close();		// 뚜껑을 닫아 줬어. 

	// 커맨드 리스트 수행
	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);	// 실행을 요청할거야. 
	// 지금까지는 CommendQueue에다가 밀어 넣는 거였고, 그걸 실질적으로 실행하는 부분은 여기서 실행하는게 되는 거. 
	// 사실 RenderBegin이라고 했지만 아직까지는 뭔가를 그리고 있던건 아냐. Queue에다가 밀어 넣어 준거고, 여기서 뭔가 한 셈이 된거.  

	_swapChain->Present();	// 앞 단에 있는 버퍼를 이용해 화면을 채워주는 부분

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	WaitSync();		// 위에서 요청한 일련의 과정들이 실행될 때까지 CPU가 대기를 하게 될거야. 
	// _fence를 이용해서 기다리는 부분이 있었어. 

	// 모든 일감이 처리가 되었으면
	_swapChain->SwapIndex();
	// 기존에 있던 _backBufferIndex를 바꿔치기 하는 거. 0번이었으면 1, 1번이었으면 0으로 바꿔치기 해서. 거꾸로 Swap할 준비를 마치게 되는 거. 

}
// 정리: 일련의 함수가 호출되면 백버퍼 인덱스가 0이었으면 1로 바뀌게 될거고 그 다음에서 RenderBegin이 똑같은 코드가 실행이 되면 1번이 앞에 가고 0번이 뒷단에 가고 바꿔치기가 왔다갔다 되는 코드가 실행이 되는거. 
// RenderBegin과 RenderEnd를 세트로 기억을 해줘야 한다. 

// commandQueue가 작업하고 있는 부분에서 SwapChain도 맞물려서 동작을 하고 있다고 보면 되는거고
// GPU에게 백버퍼를 알려줄 때 _descHeap 즉 우리가 만들어준 View를 이용해 건내주고 있음을 알 수 있어. 
// 기존에 만들었던 여러가지 기능들이 맞물려서 세트로 동작하고 있다고 볼 수 있어. 
// 최종 기능을 CommandQueue에 대표로 넣어놓긴 했는데 걔를 다시 엔진에서 꺼내서 활용하고 있는 걸 알 수 있어. 
