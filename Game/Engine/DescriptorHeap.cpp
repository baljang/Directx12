#include "pch.h"
#include "DescriptorHeap.h"
#include "SwapChain.h"

// .cpp������ <>�ȿ� class�� �־� ���漱���� �ص� �ǹ̰� ����. 
void DescriptorHeap::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain;	// ���߿� Ȱ���� �Ŵϱ� ����Ʈ ������ �����ϰ� �־�. 

	// Descriptor (DX12) = View (~DX11)
	// [������ ��]���� RTV ����
	// DX11�� RTV(RenderTargetView), DSV(DepthStencilView), 
	// CBV(ConstantBufferView), SRV(ShaderResourceView), UAV(UnorderedAccessView)�� ����!

	// �������� ���� ���ҽ����� ������ �䰡 �־���. �� ���ΰ����ϰ� �־��µ� DX12���ʹ� DescriptorHeap�� �̿��ؼ� ��� �ֵ��� �� �����ϰ� �����ϰ� �ȴٴ� ������ ����. 
	// �׸��� �ᱹ���� �츮�� ���ҽ� ��ü�� swapChain���� ���� �߾���. _renderTargets��� �ְ� �츮�� ���ҽ���� �� �� �־�. 
	// �װ��� �����ϴ� ��ȼ��� �ϳ� �� ����� �ִٰ� ���� ��. ���� ���� �� ���� �ѵ� �׷��� �԰��� ������ ����. 

	_rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	// RenderTargetView�� ����� �ϳ� ����ϰ� �־�. 
	
	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// Ÿ���� RTV�� �־��ְ� �ִ� ��
	rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;	// ��� ����� �ٰ��̳�? 2���� ����� �ּ���. 
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvDesc.NodeMask = 0;
	// �̰� ������ ä���� CreateDescriptorHeap�� ������ �Ǹ� RTV �迭�� ��������� �Ǹ�� 2��¥�� ����� ���� �� �ִ� ũ���� �迭�� ��������� �� �� �־�. 
	// _rtvHeap�� �迭���� �����̶�� �� �� �־�. 

	// descriptHeap�� ������ �迭�̶�� �� �� �־�. �迬�� �迭�ε� ���� �ֵ鳢���� ���� ���ױ׷� ������ �� �ִ� �׷� �迭�̶� �� �� �־�. 
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
		_rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * _rtvHeapSize);
		// CD�� �� �� �߰������ d3dx12.h ���̺귯���� �ִ� ���� Ŭ������ �̶�� �߾�. 
		// ���� ���� �ᱹ handle.ptr = base.ptr + offsetScaledByIncrementSize; �̰� ���ִ� ��
		// handle�� ù ���� �ּҸ� ��Ÿ���� �ִٰ� ���� ��. rtvHeapBegin.ptr = rtvHeapBegin.ptr + offsetScaledByIncrementSize; �̷� ����
		// _rtvHandle[i].ptr = rtvHeapBegin.ptr + i * _rtvHeapSize; �̷��� �ٲ�ġ�� ���ִ� �Ŷ� ��������
		// ���� �ּҿ��� i���ٰ� Heap����� �����ְڴٴ� �Ŵϱ� ù��° �����ʹ� 0�̴ϱ� ���� �ּҰ� �Ǵ°� �°� �� �������� i�� 1�� �״ϱ� HeapSize��ŭ �����༭ 
		// ���� ĭ���� �Ѿ�� �� ������ ä���ְڴ� ������ �ϴ� ��. 
		// Handle�̶�� �� ���� ���Ǵµ� Handle�� �Ϲ������� ������ó�� ���������� �����ؼ� ����� �ϴ� �����̶�� �������ָ� ��. 
		// windows api���� â�� ���� �ڵ� �̷� ��� �����. ������ �����ε� � ��ü�� ����Ű�� ��ü��� ���� ��. 
		// cpu������ ���ָ� �ñ��� ���忡���� �ڵ��� ���ؼ� � ������ �˷��� �� �ִ� �Ű�, GPU������ �������� �˾Ƶ�� �׷� ��Ȳ�� �߻��Ѱ�. 

		device->CreateRenderTargetView(swapChain->GetRenderTarget(i).Get(), nullptr, _rtvHandle[i]);
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

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetBackBufferView()
{
	return GetRTV(_swapChain->GetCurrentBackBufferIndex());
// ���� _swapChain���� CurrentBackBufferIndex�� �����ذ����� �װ��� RTV�� ������ ��ȯ�� ���ְ� �־�. 
}