#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Engine.h"

CommandQueue::~CommandQueue()
{
	::CloseHandle(_fenceEvent);	// �̺�Ʈ�� �����ϴ� �κ�
	// ���� �ʾƵ� ���α׷��� ������ ������ ������ �����Ҵ� �� delete �ϴ� ���� ���� �����ΰ�ó�� 
	// event Ȱ�� �Ŀ��� ��ȣ�� ���ִ� ��ó���� ���ִ°� �������̱⿡ �߰��� ��. 
}

void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	// device�� ���� ���� �۾��ϰ� �ִ� �� ���Ǳ�� ���� ��. 
	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue));
							// (������� ������, ���� ������)
	// cmdQueue�� new�� ����°� �ƴ϶� COM ��ü�� Ư�� �Լ��� ���� ���������� ������ְ� �־�. 
	// 3�ѻ縦 �� �̷� ������ ������־���. 

	// - D3D12_COMMAND_LIST_TYPE_DIRECT : GPU�� ���� �����ϴ� ��� ���
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));

	// GPU�� �ϳ��� �ý��ۿ����� 0����
	// DIRECT or BUNDLE
	// Allocator
	// �ʱ� ���� (�׸��� ����� nullptr ����)
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));
	// ������ ���� allocator �Ѱ��ְ� �־�. std::vector�� ������. vector���� � �Ҵ��ڸ� �̿��� �޸𸮸� �Ҵ��ϴٰ� vector�� clear�� ������ 
	// �� �޸𸮸� �α׸� �����°� �ƴ϶� capacity�� �״���ε� size�� resize�� �Ǿ� �پ���. commandQueue�� �׷� ������ �����ϰ� �־�.
	// ���߿� commandlist�� �ִ� ���� ������ �о� ���� �ٵ� �޸� �����°� �ƴ϶� allocator�� �޸� �����ϰ� �ִ� ����	  

	// CommandList�� Close / Open ���°� �ִµ�
	// Open ���¿��� Command�� �ִٰ� Close�� ���� �����ϴ� ����
	_cmdList->Close();

	// ���������� �� �ϴ����� ���߿� �ٽ� �� �� ã�ƺ��� �ϴ� �Ѿ��. 

	// CreateFence
	// - CPU�� GPU�� ����ȭ �������� ���δ�
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));	// ��Ÿ�� ������ְ� �־�. 
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	// CreateEvent ��ü�� ���� directX Ư������� �ƴϰ� �Ϲ������� ��Ƽ �����忡�� ����ȭ �� ���� ���� ���̴� ����̾�. 
	// ��ȣ�� ���� ����. �������� ���� ����ٰ� �Ķ����� �Ǹ� �ǳʰ��� �Ͱ� ���������� Event�� ������ְ� ��ȣ ���ְ� Ư�� ���� �����ϸ� �Ķ����� �����Բ� ��û
	// �Ķ� ���� ���� �� ���� ��ٸ��� ���·� ����ȭ�� ���ְ� ��. �׷� �뵵�� Ȱ��. 
}

void CommandQueue::WaitSync()
{
	// Advance the fence value to mark commands up to this fence point.
	_fenceValue++;	// �潺�� � ���� �־�. 0���� 1�� ������ ���� ��. � �ϰ��� �� ��û�� ��Ų ������ ��ȣ�� �ű�ž�. 
	// ���¿� ���� ��ȣ�� �ű� ������ 

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	_cmdQueue->Signal(_fence.Get(), _fenceValue);
	// commandQueue���� �� ��ȣ�� �ǳ��ְ� 

	// Wait until the GPU has completed commands up to this fence point.
	if (_fence->GetCompletedValue() < _fenceValue)
	{
		// Fire event when GPU hits current fence.  
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);
		// �� ��ȣ���� ���࿡ �������� fenceEvent�� ������� �ֶ� �ϰ� �ִ� ��. 
		// �� �κб����� GPU�� �����ϴٰ� ���⼭ �ǳ��� ���ڿ� �ش��ϴ� �ϰ����� ���´� ������
		// event�� Ʋ� �Ķ����� ���ְ� �ɰŰ� 

		// Wait until the GPU hits current fence event is fired.
		::WaitForSingleObject(_fenceEvent, INFINITE);
		// CPU�� ���� ��ٸ��� ��. 
		// ��ٸ��ٰ� �Ķ��� �Ǹ� �����ϰ� �Ǵ°�. 

		// �⺻������ � ���� �༭ �ش� �κ��� ���� �� ���� ��ٸ����ְ� �ִ� �κ�
		// CPU�� GPU�� ���� ���� �� ���� ��ٸ��� ����̴� ���ϱ� CPU�� � �����ε� ��� ������ ���� �ڵ��� �� �� ����. 
		// �����ϰ� ����� ���� �̷��� ����ž�. ȿ���� �ڵ�� �ƴ�. 
	}
}


void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	_cmdAlloc->Reset();	
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);
	// std::vector�������� clear ���� �Ŷ� ����. ����ϰ� �����ذ�. �޸𸮰� �پ���� �ʰ� capacity�� �״���� ����

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT, // ȭ�� ���
		D3D12_RESOURCE_STATE_RENDER_TARGET); // ���� �����
	// CD..�� ���� Ŭ������� �ߴµ� f12�� �� ���� �̷� ���� ��ɵ��� ä���ִ� �� �� �� �־�. 
	// ���������� ������ �ϴ��� �м��� �ʿ�� ���� �־��ְ� �ִ� �κ��� �ٽ��̾�. Transition�Լ������� ���� �� �ް� ������ �ᱹ����
	// 3������ ���� �ٽ����� ������. �ϰ� �ִ°� 1. � ���ҽ��� �޾��ְ� �ִ� �� �� �� �־�. SwapChain�̶� �¹����� ������ �ϰ� �ִ� �ǵ� 
	// 0���� 1���� �Դٰ��� �ٲ�ġ�� �ϸ鼭 �� �κ��� �����ϰ� �ִٰ� ���� ��. 2. PRESENT�� ȭ�� ��� 3. RENDER_TARGET�̶�� �� ���� ����� �޾��ִ� 
	// �ڿ��� �۾��� �� �� ���µ��� ���ϴµ� Transition�� Ŀ�� ��� PRESENT�� before, RENDER_TARGET�� after��� �� �� �� �־�. 
	// ���� ������� �ٲ�ġ�� �ϰڴٴ°� �� �κ��̶�� �� �� �־�. ���� ����� ���ҽ��� �̵� ���Ѽ� GPU �۾� �뵵�� Ȱ���ϰڴٰ� ��û�� ������ִ� �κ�
	// �ٷ� ���� �Ǵ°� �ƴ϶� barrior ���·� ��������µ� �װ� _cmdList->ResourceBarrier(1, &barrier);�� �о� �־���.


	// � ��å���� � �ǹ��� �Ӵ븦 �ϰڴٰ� ������ ���ٰž�. 
	_cmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get()); // �������� #include "Engine.h"����.
	// �� ������ Ȱ���� �ϰڴ� �����Ѱ�. 

	GEngine->GetCB()->Clear(); // ������Ʈ ���� Ŭ����. 

	_cmdList->ResourceBarrier(1, &barrier);	

	// Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
	// Ŀ��� ����Ʈ�� �о����� �ٽ� ������ �ؾ� �Ѵٰ� �ϴϱ� 
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);

	// Specify the buffers we are going to render to.
	// � ���ۿ��� �׸��� �׷��� �ϴ����� �ٽ� ����� ����� �ϴµ� ���⼭ ����
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView =  _swapChain->GetBackRTV();	// ����۸� �����´����� �ű⿡ ������� GPU���� �ϰ��� �׷��޶�� ��û�ϴ°�
	_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);	
	// LightSteelBlue��� ������ ����µ� �ƹ��͵� ���� ���¿��� �Ѻ��� �Ǹ� ���� �ϴû��� �߰� ��. ������ ���� �� ���� �ڵ尡 ����ǰ� �ִ��� �׽�Ʈ�� �غ��� �ɰž�. 
	_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, nullptr);	// Ouput Merger
	// GPU�� �۾��� �� ����۶�� �θ��µ� ����۸� GPU���� �˷��ִ� �κ��� ���⿡ ����. 
	// �ٲ�ġ��� �Դٰ��� �ϴ� �κ��� �۾��ϰ� �ִ� �Ű�, ���� ȭ���� �׸��� �ִ� �뵵���� Ȱ���ϰ� �ִ� �ָ� �ڿ� �ִ� ����۷� �ٲ�ġ�� �Ѵ����� GPU���� ���� ������ ������� �׷����� ��û�ϴ� �Ϸ��� ������
	// GPU���� ���ָ� �׻� ����ۿ� �ش޶�� ��û�� �� ������ ���߿� ���� �̷����� ��ü���� GPU���� ��û�ؼ� �߰������� �ϰ��� ��û���ٰŰ�
	// ���������� �ϰ��� ������ ���� ���� RenderEnd�� ȣ�����ٰǵ�
}

void CommandQueue::RenderEnd()	 
{
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, // ���� �����
		D3D12_RESOURCE_STATE_PRESENT); // ȭ�� ���
	// ������ ���·� ������ �ϴµ� ������ �ٲ�� �־�. 
	// �������� RENDER_TARGET �ڿ��� GPU�۾� �뵵�� Ȱ�� �Ǵ��ָ� ȭ����� �뵵�� �ٽ� ���ٷ� �ٲ�ġ�� ���ִ� ��Ȳ�̵Ǵ� ��. 
	// �ڿ� �ִ� �ָ� �ٽ� �մ����� �ű�� �ִ� ��Ȳ�� �Ǵ� ��. 

	_cmdList->ResourceBarrier(1, &barrier);		// �ϰ� �о� �־� �ִ°�
	_cmdList->Close();		// �Ѳ��� �ݾ� ���. 

	// Ŀ�ǵ� ����Ʈ ����
	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);	// ������ ��û�Ұž�. 
	// ���ݱ����� CommendQueue���ٰ� �о� �ִ� �ſ���, �װ� ���������� �����ϴ� �κ��� ���⼭ �����ϴ°� �Ǵ� ��. 
	// ��� RenderBegin�̶�� ������ ���������� ������ �׸��� �ִ��� �Ƴ�. Queue���ٰ� �о� �־� �ذŰ�, ���⼭ ���� �� ���� �Ȱ�.  

	_swapChain->Present();	// �� �ܿ� �ִ� ���۸� �̿��� ȭ���� ä���ִ� �κ�

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	WaitSync();		// ������ ��û�� �Ϸ��� �������� ����� ������ CPU�� ��⸦ �ϰ� �ɰž�. 
	// _fence�� �̿��ؼ� ��ٸ��� �κ��� �־���. 

	// ��� �ϰ��� ó���� �Ǿ�����
	_swapChain->SwapIndex();
	// ������ �ִ� _backBufferIndex�� �ٲ�ġ�� �ϴ� ��. 0���̾����� 1, 1���̾����� 0���� �ٲ�ġ�� �ؼ�. �Ųٷ� Swap�� �غ� ��ġ�� �Ǵ� ��. 

}
// ����: �Ϸ��� �Լ��� ȣ��Ǹ� ����� �ε����� 0�̾����� 1�� �ٲ�� �ɰŰ� �� �������� RenderBegin�� �Ȱ��� �ڵ尡 ������ �Ǹ� 1���� �տ� ���� 0���� �޴ܿ� ���� �ٲ�ġ�Ⱑ �Դٰ��� �Ǵ� �ڵ尡 ������ �Ǵ°�. 
// RenderBegin�� RenderEnd�� ��Ʈ�� ����� ����� �Ѵ�. 

// commandQueue�� �۾��ϰ� �ִ� �κп��� SwapChain�� �¹����� ������ �ϰ� �ִٰ� ���� �Ǵ°Ű�
// GPU���� ����۸� �˷��� �� _descHeap �� �츮�� ������� View�� �̿��� �ǳ��ְ� ������ �� �� �־�. 
// ������ ������� �������� ��ɵ��� �¹����� ��Ʈ�� �����ϰ� �ִٰ� �� �� �־�. 
// ���� ����� CommandQueue�� ��ǥ�� �־���� �ߴµ� �¸� �ٽ� �������� ������ Ȱ���ϰ� �ִ� �� �� �� �־�. 
