#include "pch.h"
#include "Mesh.h"
#include "Engine.h"

void Mesh::Init(vector<Vertex>& vec)	// vector에 Vertex를 받아 줄거야. 삼각형이면 Vertex를 3개 받아주면 됨. 
{										// Vertex에 위치정보, 칼라정보를 넣어준 것을 볼 수 있어. 왜 굳이 컬러까지 받느냐? 삼각형 점 3개를 각각 다른 색으로 만드는 실습 해볼거야. 
										// Mesh라는 거 자체가 정점마다 색상을 가지고 있어서 중간에 있는 점들을 적절히 보관이 되어서 중간색을 갖고 있는 형태로 스르륵 색상이 변화하는 모습 보게 될거야. 
	_vertexCount = static_cast<uint32>(vec.size());	// 정점 목록 저장. Render할 때 _vertexCount 뱉어줘야 하기 때문에 
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	// CD3DX12 헬퍼 기능 사용하고 있어. d3dx12.h에 만들어져있는 기능들 
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);	// 어떤 용도로 활용할것이냐. 타입 업로드라고 요청을 해놨어. 
	// 여러 타입이 있는데 UPLOAD 타입으로 만들어주면, 걔는 시스템 메모리, CPU쪽에서 작업을 한거를 GPU쪽에다가 넘겨주는 용도로만 활용을 해야 되기는 하는데 
	// 사실은 버퍼를 2개를 만들어서 하나는 디폴트 버퍼를 만들어주고, 하나는 업로드 버퍼를 만들어줘서, 업로드 버퍼는 임시로 데이터를 복사하는 용도로만 활용을 해야되는게 맞긴 한데 
	// 어떤 문서에는 UPLOAD를 공용으로 활용하는 경우도 있어. 이런 식으로 데이터를 전달하는 용도, GPU에서 해당 버퍼를 참고하여 계속 활용하는 용도로도 사용을 하기는 함. 
	// 일단 지금은 공용으로 활용을 할거야. 버퍼를 만들어서 거기에 데이터를 건내준 다음에 GPU에서도 그 버퍼를 참고해서 계속 사용을 하라고 1석 2조로 업로드 타입으로 만들어 줄거야. 
	// MSN 문서를 보면 분리해서 디폴트 타입으로 만들어주고 데이터를 전송할 때만 업로드를 만들어주라고 되어 있기는 한데 그건 사소한 디테일이라고 생각하고 지금은 편리하게 만들어 볼 것. 
	// 어떤 용도로 활용할지를 property로 만들어 주고 있고, 타고 가면 굉장히 설정할 수 있는게 많아. 기본적으로 단순한 형태로 어떤 타입인지만 지정해주고 나머지는 기본형태로 활용을 한다고 볼 수 있어. 
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);	// 사이즈 만큼 버퍼를 만들고 싶다. 버퍼를 요구 한거. 
	// 가장 기본적인 형태로 2개를 설정한 다음에 CreateCommittedResource로 리소스를 만들어준거. 

	DEVICE->CreateCommittedResource(	// 정점들을 GPU에 복사를 해줘야 해. 해당 공간을 할당을 해주는 거
		&heapProperty,					// GPU쪽에 가서 어떤 공간을 할당을 해서 필요한 정보를 넣어 줄 테니까 잠시 창고를 빌려줘 라는 거. 
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer));	// 이런식으로 _vertexBuffer가 만들어졌는데, 우리의 RAM에 들어가 있는 공간이 아니라 엄밀히 말하면 원격으로 GPU안에 있는 공간을 말하는 거.  
	// 그러기 때문에 공간에 데이터를 복사하려면 막바로 복사할 수는 없고 이런식으로 두단계를 거쳐서 만들고 있는 거. 


	// Copy the triangle data to the vertex buffer.
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.

	// 일단 Map이라는 함수를 만들어서 연결을 해주고 있는 거. 
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);
	// 두번째 인자로 넘겨주는 vertexDataBuffer를 통해가지고 잠시나마 거기에 데이터를 복사할 수 있게 연계를 해준거라고 볼 수 있어. 

	::memcpy(vertexDataBuffer, &vec[0], bufferSize);
	//vertexDataBuffer에다가 데이터를 밀어넣어 준 다음에 

	_vertexBuffer->Unmap(0, nullptr);
	// 다시 Unmap을 해가지고 뚜껑을 뿅 닫아주면 
	// vertexDataBuffer안에 카피를 해준 정보가 실제로 GPU 쪽 메모리에 복사가 된다는 거 알 수 있어.   
	
	// RootSignature에서 외주를 준다고 했어. 한국에서 원격으로 외주를 맡기는 나라에 이런저런 리소스를 복사를 하고 시작을 해야 한다고 했었는데 
	// 지금 이런 식으로 Vertex라는 거 자체로 어떤식으로든 저쪽에다가 갖다가 놓은 다음에 거기서 작업을 하는 개념이라 이런 식으로 
	// 중간 버퍼로 하나 공간을 만들어서 거기다가 복사를 하는 작업을 해줬다고 볼 수 있어. 

	// Initialize the vertex buffer view.
	// view를 만들어 줬는데 각각 
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();	// 어디에 위치해 있는지 
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // 정점 1개 크기			// 크기가 얼마짜리 인지 
	_vertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기	
	// 이런식으로 관리를 해주고 있어. 

	// 뚜껑이 unmap으로 닫힌 상태니까 CPU쪽에서 베트남[		] 메모리에 어떻게 접근을 해야 할지 알 수 없고, view를 통해서 묘사한다고 볼 수 있어. 
	// 베트남[		]를 사용하겠다 하면 view를 건내주게 되면 GPU쪽에서도 view라는 걸 이용해서 어떤 데이터를 가리키고 말을 하는 건지 찰떡같이 알아듣는다고 볼 수 있어. 
}


void Mesh::Render()
{
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	
	// 지금 정점들이 어떤 형태로 연결이 되어 있는지 기본적으로 삼각형으로 연결이 되어 있다. 타입이 많아. 일반적으로는 TriangleList로 활용해주면 됨. 
	
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15)
	// 슬롯이랑 여러가지 정해줄 수 있는데 슬롯은 0부터 15번 정해주면 되는데 지금은 0으로 통일해서 넣어줌.
	// vertexBufferView는 아까 만들어준 리소스를 가리키고 있는 뷰. 묘사하고 있는 뷰. 이걸 이용해서 데이터를 활용할거라고 설명을 해주느거

	// 루트 시그니쳐는 임대만 한 것이니 여기서는 실질적으로 밀어 넣는 작업 해야 해. 
	// TODO: 인자를 세팅하는 부분
	// 1) Buffer에다가 데이터 세팅
	// 2) Buffer의 주소를 register에다가 전송

	//CMD_LIST->SetGraphicsRootConstantBufferView(0, ??) 이걸 당장 처리할 수는 없어. 문제가 있어. 이게 지금 DX12에서는 CommandQueue를 활용하고 있어. 
	// 얘가 2)를 담당하는 거 

	// 행동 일어났을 때 바로바로 해달라는게 아니라 일감 모아 놨다가 RenderEnd 실행하는 순가 모인 일감들을 한꺼번에 수행하고 있어. 
	// 서버에서 이 커멘드 패턴 활용하고 있어. 근데 이런식으로 커맨드 패턴으로 바꿔서 queue에 넣는 방식으로 하면 코딩이 조금 더 어려워질 수 밖에 없어. 
	// 유의 깊게 봐야하는 건 데이터를 복사하는 부분은 CommandList를 이용하는게 아니라 그냥 DEVICE를 통해서 소스 만들어서 거기에 Map을 만들어 때려 넣었어. 
	// 이제부터 Device를 통해서 뭔가가 이루어 질때는 CommandQueue를 이용해서 나중에 이루어지겠다는 게 아니라 지금 당장 뭔가를 이루어지고 있다고 생각하면 됨. 
	// 반면 CommandList에서 뭔가 작업하는 것은 그냥 예약만 하는거고, 일감 자체는 나중에 Exit을 할 때 한꺼번에 일어난다는 차이가 있어. 
	// 실행시점을 왜 이렇게 강조하느냐. 사실 이게 굉장히 중요. 많은 차이 일으키게 됨. 

	// 두번째 인자를 보면 버퍼의 위치를 묻고 있다는 걸 알 수 있어.

	GEngine->GetCB()->PushData(0, &_transform, sizeof(_transform)); // 0번 슬롯에 _transform에 있는 데이터를 넣어달라 요청
	GEngine->GetCB()->PushData(1, &_transform, sizeof(_transform));	// 1번 슬롯에도 같은 거 넣어줬어. 
	// 1번에다가 데이터를 넣어준 다음에 b0에다가 보내달라고 요청을 한거고, 2번에다가 데이터를 넣어준다음에 b1에다가 데이터의 주소를 밀어 넣어 달라고 요청을 한 셈이 된거야. 

	CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);
	// 그려주는 작업 예약하고 있는 거. 
	// 예약한 다음에 묶어가지고 CommandQueue의 RenderEnd가 실행되는 순간 이런 부분들이 같이 실행이 된다.  
}

// 이렇게 Init하는 부분과 그려주는 부분이 나눠졌는데 그려지는 부분은 CMD_LIST활용하게 됨. 
// CommandQueue를 다시 한번 살펴보면, RenderBegin, RenderEnd라는 걸 이렇게 해줬어. 
// RenderBegin은 초기화 하는 부분을 막 한 다음에 백버퍼 설정 등등이 들어가서 그리기를 시작할거야. 
// _cmdList에다가 다음에 필요한 명령어들을 딱 밀어넣어주는데 그 시점에서 이 Render가 실행이 된다. 
// RenderBegin과 RenderEnd 사이에 이런 나머지 렌더링 부분들이 계속 추가가 되면서 CommandList에다가 하나하나씩 우리가 명령어들을 넣어준다고 보면 되는 거고, 
// 마지막으로 모든 준비가 끝났다면 RenderEnd가 호출이 되는 순간 CmdList닫아주고 더이상 이번 프레임은 마감하겠습니다. 닫아준 다음에 
// 실질적으로 수행하는 부분이 들어가고 있었어. 

