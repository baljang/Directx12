#include "pch.h"
#include "SwapChain.h"
//#include "Engine.h"	// Engine�� �鿩�� ������ Device, CommandQueue, SwapChain 3�ѻ� �� Ȱ���� Ȯ�� ������ �ʿ��� �� �Ѷ� �Ѷ� �߰��ϴ� �� �ǰ��� �� �־�. 
//#include "Device.h"

void SwapChain::Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	CreateSwapChain(info, dxgi, cmdQueue);
	CreateRTV(device); 
}

void SwapChain::Present()	// ���� ȭ�鿡 � �ְ� ��µǾ�� �ϴ��� ���� �Ǹ� �׷��� �ϴ� �κ�
{
	// Present the frame.
	_swapChain->Present(0, 0);	
}

void SwapChain::SwapIndex()		// �� �״�� index�� �����Ѵ� 0�̸� 1, 1�̸� 0
{
	_backBufferIndex = (_backBufferIndex + 1) % SWAP_CHAIN_BUFFER_COUNT;
}

void SwapChain::CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	// ������ ���� ���� ������
	_swapChain.Reset();	// Init �ι� ���� ��� ������ �����°ǵ� ���� ��� �Ǳ� ��. 

	DXGI_SWAP_CHAIN_DESC sd;	// swapchain�� ���� ���縦 ���ְ� �ִ� ��. ��� ���� ���̴�.
	// ���̵� ȭ�鿡 �׸��� �� �׸��� �׷��ִ� ���̴� ���ϱ� ȭ�� ũ��� �Ȱ��� ������ִ� �� �� �� �־�. 
	// ���̸� ��� ����� �� ������ ���⼭ ������ �ִ� ��.�� 
	sd.BufferDesc.Width = static_cast<uint32>(info.width); // ������ �ػ� �ʺ�
	sd.BufferDesc.Height = static_cast<uint32>(info.height); // ������ �ػ� ����
	sd.BufferDesc.RefreshRate.Numerator = 60; // ȭ�� ���� ����
	sd.BufferDesc.RefreshRate.Denominator = 1; // ȭ�� ���� ����
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ������ ���÷��� ����, 8��Ʈ�� Ȱ���� �ϰ� �ִ�. ���������� 32��Ʈ Ȱ���ϴ� �� ������ �� �־�. 
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1; // ��Ƽ ���ø� OFF <- ���߿� �ٷ�� �ɰž�. 
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �ĸ� ���ۿ� �������� �� 
	sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT; // ����+�ĸ� ����, �Ʊ� 2��� �����ߴ� ��
	sd.OutputWindow = info.hwnd;
	sd.Windowed = info.windowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // ���� �ĸ� ���� ��ü �� ���� ������ ���� ����
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	dxgi->CreateSwapChain(cmdQueue.Get(), &sd, &_swapChain);
	// commandQueue�̷� �ֵ��� device�� ���� ������µ� dxgi�� ���ؼ� ������� �� �� �� �־�. 
	// dxgi�� ��°� ���õ� �κе��� �� �־��µ� swapchain ���� ��쿡�� �׷��Ƚ��� Ȱ���� ���� ����ϴ� ������ �ƴϴ� ���� ���⿡ �� �־�. 
	// ������� ������ _swapCahin�� �־� �޶�� ��û�� ���ذ�. 

	for (int32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		_swapChain->GetBuffer(i, IID_PPV_ARGS(&_rtvBuffer[i]));
	// swapChain�� ������ְ����� �츮�� ��û�� ���۵� ���� ������ְ� �� �ž�. 
	// GetBuffer�� �̿��ؼ� _renderTargets��� ���̿��� �ٽ� ������ ���ִ� ���� �� �� �־�. 
	// swapChain�� ����� �ְ� ���������� ���۵� ����� �����ϱ� �� ���۸� �ٽ� ������ ���� ����� �Ŵϱ� _renderTargets�̶�� �κп� ������ �ϰ� �ִ°�. 
	// &_renderTargets[i]�� _rtvBuffer�� �̸��� �ٲ�����. 

}


// DescriptorHeap�� �ִ� �ڵ�
// .cpp������ <>�ȿ� class�� �־� ���漱���� �ص� �ǹ̰� ����. 
void SwapChain::CreateRTV(ComPtr<ID3D12Device> device)
{
	// Descriptor (DX12) = View (~DX11)
	// [������ ��]���� RTV ����
	// DX11�� RTV(RenderTargetView), DSV(DepthStencilView), 
	// CBV(ConstantBufferView), SRV(ShaderResourceView), UAV(UnorderedAccessView)�� ����!

	// �������� ���� ���ҽ����� ������ �䰡 �־���. �� ���ΰ����ϰ� �־��µ� DX12���ʹ� DescriptorHeap�� �̿��ؼ� ��� �ֵ��� �� �����ϰ� �����ϰ� �ȴٴ� ������ ����. 
	// �׸��� �ᱹ���� �츮�� ���ҽ� ��ü�� swapChain���� ���� �߾���. _renderTargets��� �ְ� �츮�� ���ҽ���� �� �� �־�. 
	// �װ��� �����ϴ� ��ȼ��� �ϳ� �� ����� �ִٰ� ���� ��. ���� ���� �� ���� �ѵ� �׷��� �԰��� ������ ����. 

	int32 rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);	// ���ο� _rtvHeapSize�ȵ�� �ֱ�� ������ int32�� ���� �޸𸮿� ��� Ȱ���ϰ� �����ֱ��
	// RenderTargetView�� ����� �ϳ� ����ϰ� �־�. 

	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// Ÿ���� RTV�� �־��ְ� �ִ� ��
	rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;	// ��� ����� �ٰ��̳�? 2���� ����� �ּ���. 
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvDesc.NodeMask = 0;
	// �̰� ������ ä���� CreateDescriptorHeap�� ������ �Ǹ� RTV �迭�� ��������� �Ǹ�� 2��¥�� ����� ���� �� �ִ� ũ���� �迭�� ��������� �� �� �־�. 
	// _rtvHeap�� �迭���� �����̶�� �� �� �־�. 

	// descriptHeap�� ������ �迭�̶�� �� �� �־�. �迭�� �迭�ε� ���� �ֵ鳢���� ���� ���ױ׷� ������ �� �ִ� �׷� �迭�̶� �� �� �־�. 
	// ���� ������ �����ͳ��� �迭�� ����
	// RTV ��� : [ ] [ ] 
	device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&_rtvHeap));

	// �迭 ���� ���빰�� ä���ֱ� ���� �Ʒ� �ڵ尡 ����ȴ�. 
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = _rtvHeap->GetCPUDescriptorHandleForHeapStart();
	// �ּҸ� �����ְ� �ɰž�. 
	// D3D12_CPU_DESCRIPTOR_HANDLE�� ���� ���캸�� SIZE_T ptr; �̶�� � �ּҰ��� ���� �ִ� �� �� �� �־�. 
	// �ٸ� �����ʹ� �ƴϰ� � ���� ���·� �ּҸ� �����ϰ� �ִ� �� �� �� �־�. 
	// �갡 RTV ����� ���� �ּҸ� ����Ű�� �ִٰ� �� �� �־�. 

	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)	// 2�� loop
	{
//		_rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * _rtvHeapSize);
		_rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * rtvHeapSize);
		// CD�� �� �� �߰������ d3dx12.h ���̺귯���� �ִ� ���� Ŭ������ �̶�� �߾�. 
		// ���� ���� �ᱹ handle.ptr = base.ptr + offsetScaledByIncrementSize; �̰� ���ִ� ��
		// handle�� ù ���� �ּҸ� ��Ÿ���� �ִٰ� ���� ��. rtvHeapBegin.ptr = rtvHeapBegin.ptr + offsetScaledByIncrementSize; �̷� ����
		// _rtvHandle[i].ptr = rtvHeapBegin.ptr + i * _rtvHeapSize; �̷��� �ٲ�ġ�� ���ִ� �Ŷ� ��������
		// ���� �ּҿ��� i���ٰ� Heap����� �����ְڴٴ� �Ŵϱ� ù��° �����ʹ� 0�̴ϱ� ���� �ּҰ� �Ǵ°� �°� �� �������� i�� 1�� �״ϱ� HeapSize��ŭ �����༭ 
		// ���� ĭ���� �Ѿ�� �� ������ ä���ְڴ� ������ �ϴ� ��. 
		// Handle�̶�� �� ���� ���Ǵµ� Handle�� �Ϲ������� ������ó�� ���������� �����ؼ� ����� �ϴ� �����̶�� �������ָ� ��. 
		// windows api���� â�� ���� �ڵ� �̷� ��� �����. ������ �����ε� � ��ü�� ����Ű�� ��ü��� ���� ��. 
		// cpu������ ���ָ� �ñ��� ���忡���� �ڵ��� ���ؼ� � ������ �˷��� �� �ִ� �Ű�, GPU������ �������� �˾Ƶ�� �׷� ��Ȳ�� �߻��Ѱ�. 

//		device->CreateRenderTargetView(swapChain->GetRenderTarget(i).Get(), nullptr, _rtvHandle[i]);
		device->CreateRenderTargetView(_rtvBuffer[i].Get(), nullptr, _rtvHandle[i]);	// DescriptorHeap�ڵ带 SwapChain�� �ű� ���̹Ƿ� SwapChain�� _rtvBuffer�� �ٷ� �� �� �ְ� ��. 
	}
	// ������ ���ؼ� ��¥ �����Ͱ� �������� �������� �����ؼ� view�� ����� �شٰ� �� �� �־�. 
	// ���� Ÿ���� ��� ����� ������ ������ �غ��� SwapChain �ʿ� �־� ������. 
	// ComPtr<ID3D12Resource>	_renderTargets[SWAP_CHAIN_BUFFER_COUNT]; �긦 �����ϰ� �ִ� �� 
	// 0��, 1�� RenderTarget�� �����ͼ� CreateRenderTargetView�� RTV�� ����� �ִ� ��. 

	// �̷��� �쿩���� ���� CreateRenderTargetView�� ���� ��
	// _rtvHandle[i]�ּҿ� ������ �ؼ� �� ������ swapChain->GetRenderTarget(i).Get() �� �����ͷ� ä����� ��� �ϴ� ��. 

	// ������� ������ �Ǹ� �ᱹ GPU���� view��� �� �Ѱ��� �� �ְԲ� �غ� �� ���°� �Ǿ��ٰ� ���� ��. 
	// ��ȼ� ���� ����. ���� ��Ȳ�� ���������̴� ��°� �� view�� ���� ������ ���ָ� �Ǵ� ��. 
}