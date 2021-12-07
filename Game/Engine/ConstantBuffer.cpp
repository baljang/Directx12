#include "pch.h"
#include "ConstantBuffer.h"
#include "Engine.h"

ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()	// 생성자와 소멸자는 별다른 내용이 없어. 
{
	if (_cbvBuffer)
	{
		if (_cbvBuffer != nullptr)
			_cbvBuffer->Unmap(0, nullptr);	// 더이상 필요 없다 하면 뚜껑 닫아주는 부분
											// 굳이 안해도 constant buffer가 소멸 될 때는 프로그램이 종료되기 때문에 안해도 큰일 나진 않아. 
		_cbvBuffer = nullptr;
	}
}


void ConstantBuffer::Init(uint32 size, uint32 count)
{
	// 상수 버퍼는 256 바이트 배수로 만들어야 한다. 원칙상 그래
	// 0 256 512 768
	// 매번 지켜서 만들 보장이 없으니까 고정해주는 부분이 여기 들어가는거. 
	_elementSize = (size + 255) & ~255;
	_elementCount = count;

	CreateBuffer();	// 버퍼 만들어주는 부분. 나중에 이런저런 내용 추가될 수 있으니까 기능 분리해줌.  
}

void ConstantBuffer::CreateBuffer()
{
	uint32 bufferSize = _elementSize * _elementCount;	// 하나의 요소의 크기에 그 요소가 몇개 있는지 곱해서 버퍼 사이즈를 크게 할당을 맏음
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);	// 업로드 타입 버퍼
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);	// 버퍼 사이즈 만큼으로 만들어주면

	DEVICE->CreateCommittedResource(	// device를 통해서 만들어줌.
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
	_currentIndex = 0;		// 그냥 0으로 밀어주는거. 어디까지 데이터를 사용하고 다음에는 어디껄 사용할지 관리하는 아이인데 이제 매 프레임마다 초기화 되어 다시 0으로 해주면서 왔따 갔다 함.
}

void ConstantBuffer::PushData(int32 rootParamIndex, void* buffer, uint32 size)
{	// rootParamIndex로 받아주는 것은 우리가 처음에 슬롯번호를 0번 -> b0 -> CBV, 1번->b1->CBV 이렇게 정해 줬었어. 
	// 0번과 1번 중에서 어떤걸 활용할지 골라주는 거.  

	assert(_currentIndex < _elementSize);	// 조건 만족 안하면 crash 내는 디버깅 코드. 버퍼의 크기만큼을 벗어나는지 체크 

	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);	// 해당 인덱스에다가 요청해준 버퍼를 복사해 넣어주고 있어. 

	D3D12_GPU_VIRTUAL_ADDRESS address = GetGpuVirtualAddress(_currentIndex);
	// 어떤 아이한테 내가 지금 주소를 계산해서 보내줘야 하는데 주소 자체는 계산식이 복잡하진 않아. 

	CMD_LIST->SetGraphicsRootConstantBufferView(rootParamIndex, address);	// 요 주소를 참고해 작업을 해주세요. 레지스터에다가 올려주는 부분.  
	// GPU의 가상 어드레스를 이용해서 호출해주고 있어. 

	_currentIndex++;
	// 하나를 사용한 다음엔 currentIndex를 1 증가시켜줘가지고, 다음 데이터를 사용하게끔 유도를 해주고 있다. 
}
// 결국 이 PushData만 호출해주게 되면 사실상 신경 쓸 필요 없이 즉시 버퍼에 데이터를 복사했다가 나중에 registers에 밀어 넣는 작업을 1+1으로 같이 처리해준다는 거 알 수 있어.  
// 버퍼를 여러개 안심해서 만들어 놨으니까 이전 데이터가 덮어써지지 않을 거란 걸 알 수 있어. 

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(uint32 index)	// GPU의 가상 주소를 구해주는 부분
{
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = _cbvBuffer->GetGPUVirtualAddress();	// 시작 주소를 받아 온 다음에 
	objCBAddress += index * _elementSize;	// 어기에 몇번째 인덱스인지를 곱해가지고 계산을 해주고 있어. 
	return objCBAddress;					// 모든 버퍼들이 옹기종기 모여서 띄엄띄엄 있는게 아니라 일종의 배열 형태로 연이어 있을 거라고 가정할 수 있어. 
}