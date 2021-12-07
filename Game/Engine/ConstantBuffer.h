#pragma once

// 얘의 역할은 Buffer 를 만들어주는 역할
// 그래서 지금 내가 어디까지 사용하고 있는지를 추적을 해가지고 누군가가 데이터를 밀어넣어주면 내가 사용하고 있는
// 영역은 건드리지 않고 다음 영역데 데이터를 밀어 넣어주고, 그 데이터를 위로 예약을 하는 식으로 동작을 한다. 

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(uint32 size, uint32 count);

	void Clear();
	void PushData(int32 rootParamIndex, void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);

private:
	void CreateBuffer();


private: 
	ComPtr<ID3D12Resource>	_cbvBuffer;		// ComPtr이 붙으면 왠만하면 GPU쪽 데이터라고 볼 수 있어. 얘를 만들어주면 Buffer가 만들어져. 
	BYTE*					_mappedBuffer = nullptr;	// 우리가 CPU쪽에서 데이터를 밀어넣을 때 사용하는 버퍼, 뚜껑을 열 때 사용하는 부분
	// Mesh에서 살펴본 memcpy를 하기 위한 버퍼 역할

	uint32					_elementSize = 0;	// 각 요소의 크기와 
	uint32					_elementCount = 0;	// 각 요소의 개수
	// 각 버퍼에 크기가 있을 거고 여러개를 만들어야 하는데 카운트와 하나의 상자의 크기를 얘기하는거. 
	// 버퍼는 버퍼 하나를 이야기 하는게 아니라 이 모든 파란색 영역의 모음집 자체를 cbv 버퍼라고 이야기 하고 있다고 볼 수 있어. 

	uint32					_currentIndex = 0;
	// 어디까지 사용했는지 추적하고 있는 거. 0부터 시작해서 늘어나게 되고 렌더링이 한 프레임을 그렸으면 다시 0으로 초기화해서 다음 프레임을 그릴 수 있게 됨. 
	// 하나씩 그리다가 모든 그림이 그려졌으면 다시 0번부터 그 다음 데이터를 받아주는 거. 

	// 이렇게 필요한 준비물들을 구성해 놓음. 
};

