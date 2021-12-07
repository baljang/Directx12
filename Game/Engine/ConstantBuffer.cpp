#include "pch.h"
#include "ConstantBuffer.h"
#include "Engine.h"

ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()	// �����ڿ� �Ҹ��ڴ� ���ٸ� ������ ����. 
{
	if (_cbvBuffer)
	{
		if (_cbvBuffer != nullptr)
			_cbvBuffer->Unmap(0, nullptr);	// ���̻� �ʿ� ���� �ϸ� �Ѳ� �ݾ��ִ� �κ�
											// ���� ���ص� constant buffer�� �Ҹ� �� ���� ���α׷��� ����Ǳ� ������ ���ص� ū�� ���� �ʾ�. 
		_cbvBuffer = nullptr;
	}
}


void ConstantBuffer::Init(uint32 size, uint32 count)
{
	// ��� ���۴� 256 ����Ʈ ����� ������ �Ѵ�. ��Ģ�� �׷�
	// 0 256 512 768
	// �Ź� ���Ѽ� ���� ������ �����ϱ� �������ִ� �κ��� ���� ���°�. 
	_elementSize = (size + 255) & ~255;
	_elementCount = count;

	CreateBuffer();	// ���� ������ִ� �κ�. ���߿� �̷����� ���� �߰��� �� �����ϱ� ��� �и�����.  
}

void ConstantBuffer::CreateBuffer()
{
	uint32 bufferSize = _elementSize * _elementCount;	// �ϳ��� ����� ũ�⿡ �� ��Ұ� � �ִ��� ���ؼ� ���� ����� ũ�� �Ҵ��� ����
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);	// ���ε� Ÿ�� ����
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);	// ���� ������ ��ŭ���� ������ָ�

	DEVICE->CreateCommittedResource(	// device�� ���ؼ� �������.
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_cbvBuffer));

	_cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedBuffer));
	// We do not need to unmap until we are done with the resource.  However, we must not write to
	// the resource while it is in use by the GPU (so we must use synchronization techniques).
}

void ConstantBuffer::Clear()
{
	_currentIndex = 0;		// �׳� 0���� �о��ִ°�. ������ �����͸� ����ϰ� �������� ��� ������� �����ϴ� �����ε� ���� �� �����Ӹ��� �ʱ�ȭ �Ǿ� �ٽ� 0���� ���ָ鼭 �Ե� ���� ��.
}

void ConstantBuffer::PushData(int32 rootParamIndex, void* buffer, uint32 size)
{	// rootParamIndex�� �޾��ִ� ���� �츮�� ó���� ���Թ�ȣ�� 0�� -> b0 -> CBV, 1��->b1->CBV �̷��� ���� �����. 
	// 0���� 1�� �߿��� ��� Ȱ������ ����ִ� ��.  

	assert(_currentIndex < _elementSize);	// ���� ���� ���ϸ� crash ���� ����� �ڵ�. ������ ũ�⸸ŭ�� ������� üũ 

	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);	// �ش� �ε������ٰ� ��û���� ���۸� ������ �־��ְ� �־�. 

	D3D12_GPU_VIRTUAL_ADDRESS address = GetGpuVirtualAddress(_currentIndex);
	// � �������� ���� ���� �ּҸ� ����ؼ� ������� �ϴµ� �ּ� ��ü�� ������ �������� �ʾ�. 

	CMD_LIST->SetGraphicsRootConstantBufferView(rootParamIndex, address);	// �� �ּҸ� ������ �۾��� ���ּ���. �������Ϳ��ٰ� �÷��ִ� �κ�.  
	// GPU�� ���� ��巹���� �̿��ؼ� ȣ�����ְ� �־�. 

	_currentIndex++;
	// �ϳ��� ����� ������ currentIndex�� 1 ���������డ����, ���� �����͸� ����ϰԲ� ������ ���ְ� �ִ�. 
}
// �ᱹ �� PushData�� ȣ�����ְ� �Ǹ� ��ǻ� �Ű� �� �ʿ� ���� ��� ���ۿ� �����͸� �����ߴٰ� ���߿� registers�� �о� �ִ� �۾��� 1+1���� ���� ó�����شٴ� �� �� �� �־�.  
// ���۸� ������ �Ƚ��ؼ� ����� �����ϱ� ���� �����Ͱ� ��������� ���� �Ŷ� �� �� �� �־�. 

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(uint32 index)	// GPU�� ���� �ּҸ� �����ִ� �κ�
{
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = _cbvBuffer->GetGPUVirtualAddress();	// ���� �ּҸ� �޾� �� ������ 
	objCBAddress += index * _elementSize;	// ��⿡ ���° �ε��������� ���ذ����� ����� ���ְ� �־�. 
	return objCBAddress;					// ��� ���۵��� �˱����� �𿩼� ������ �ִ°� �ƴ϶� ������ �迭 ���·� ���̾� ���� �Ŷ�� ������ �� �־�. 
}