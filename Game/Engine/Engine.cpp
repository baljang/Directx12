#include "pch.h"
#include "Engine.h"

void Engine::Init(const WindowInfo& info)
{
	_window = info;
	ResizeWindow(info.width, info.height);

	// �׷��� ȭ�� ũ�⸦ �����ϴ� �κ�
	_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);
				   // d3dx12.h�� ������ �Ǿ� �ִ� �κ�. 

	_device = make_shared<Device>(); 
	_cmdQueue = make_shared<CommandQueue>(); 
	_swapChain = make_shared<SwapChain>(); 
	_rootSignature = make_shared<RootSignature>(); 
	_cb = make_shared<ConstantBuffer>(); 

	_device->Init();	// �������� ������ ���漱���� ������ ��� �����Ǿ� �ִ��� �Ⱦ˷��༭. ��� �߰����ָ� ��.  
	_cmdQueue->Init(_device->GetDevice(), _swapChain); 
	_swapChain->Init(info, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init(_device->GetDevice());		// �������� ���� �� �� �ֱ� �ѵ� ���� �迡 Init���� �ǳ��ִ� ������� ����. 
	_cb->Init(sizeof(Transform), 256);	// � ������� ������ֳİ� ����ε� ���̴��� �����ָ� ��. float4
	// �Ϲ������� ��ü���� �Ѱ��ִ� ������ ��κ� ��ġ������ ���õ� ������ �Ѱ��ٶ��� ����. � ��ġ�� �ְ�, ȸ���� �ϰ� transform ������ �Ѱ��ִ� ��찡 ���Ƽ� �ӽ������� Transform�̶�� ���߾�. 
	// struct Transfrom�� ũ�Ⱑ ũ�� ������ �⺻������ 256����� �Ǿ� �Ǵϱ� 256����Ʈ�� ���� ���� ������ �� �ְ�, �ι�° �Ķ���� ���ڴ� �˳��ϰ� 256���� ��ƺ����� �Ұž�. 
	// ��� ��鰡�� ���� ��찡 �ٶ������� �ʾ�. �������� ��ü�� ���� �׷��� �Ǵ� �� �ٶ������� �ʰ�, �ʹ� �þ�� �Ǹ� ȿ�������� �ʰ� ������ �� �־�. 
}

void Engine::Render()
{
	RenderBegin();

	// TODO : ������ ��ü�� �׷��ش�

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

void Engine::ResizeWindow(int32 width, int32 height)	// Ȥ�� ũ�⸦ �ٲ� �� ������ ���ڸ� �޾��ִ� �������� �������.
{
	_window.width = width; 
	_window.height = height; 

	RECT rect = { 0, 0, width, height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);	// window API��. �������� ũ�⸦ ������ ������ 
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0);	// ������ �������� ���� ���ϴ� ��ġ�� ������ ���ְڴٴ� ��. 
					// ������ hwnd�� �̿��ؼ� ������ â�� width�� heightũ��� ������ ������ ��ġ�� 100, 100���� ������ ��� ����. 
	// ::�� �۷ι� ���ӽ����̽����� �Լ��� ã���ְڴٴ� �ǹ�. ���� ������ ���� ������
	// :: ���̸� ������ �Ϲ����� �Լ��� �ƴ϶� �ִ��� ���̺귯������ �����ϴ� �������� ���õ� ����̶�� �� �Ͻ��ϰ� �ִٰ� ���� ��. 
}
