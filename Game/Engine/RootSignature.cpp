#include "pch.h"
#include "RootSignature.h"

void RootSignature::Init(ComPtr<ID3D12Device> device)
{
	CD3DX12_ROOT_PARAMETER param[2];	
	param[0].InitAsConstantBufferView(0); // 0번 -> b0 -> CBV 
	param[1].InitAsConstantBufferView(1); // 1번 -> b1 -> CBV
	// 그림으로 보면 초록색인 root CBV를 사용하겠다고 선언을 해주는 거고
	// 위의 그림에서는 0,1,2라고 되어 있지만 초록색 2개를 만들어 줄거야. 0,1이 다 root CBV가 될거고 
	// 그 다음에 여기의 번호가 골라지게 되는데 그 부분을 넣어주고 있어. 
	// ()에 넣어줄 shader register 자체가 어떤 번호를 사용할 것인지를 정해주는데 0을 넣으면 b0를 사용하겠다는 얘기고 
	// 1번을 넣었으면 b1를 사용하겠다는 의미야. 겹치면 안돼. 중복되지 않게 골라줘야 해. 
	// 들어가는 순서대로 설정이 되기 때문에 0번 슬롯에 b0를 활용할 것이고 그 용도는 CBV를 이용할것이다. 이런 descriptor를 활용할것이다라고 선언을 해준거. 
	// 나머지 인자들은 딱히 중요하진 않는데, registerSpace 같은 경우에는 어떤 사유에 의해 이름을 겹치게 만들었다 가정할 때,
	// 겹쳤음에도 두번째 인자인 register space를 다르게 하면 얘네들을 구분해줄 수 있기는 함. 
	// b0-0, b0-1 이런 느낌으로 한번더 분리하는 느낌이지만 굳이 활용할 일은 없을거야.  
	// 마지막 인자는 shader visiblity라는 인자. 기본적으로 all 로 되어 있는데 왜 필요하느냐. 마소 pipeline 그림 보면 왼쪽에서 꽂이는 형태로 들어가게 됨. 넘기는 인자가 꽂히게 될텐데  
	// 기본적으로 특정 단계에서만 필요하다고 딱 한개만 놓으면, vertexShader에서만 쓰겠다고 하면 단계 넘어갈 때 소실이 된다. 
	// 근데 우리는 visiblity all 로 했으니까 소멸되는게 아니라 언제 어디서든 활용될 수 있게 모든 단계에서 꽂아주고 있는 상태라고 볼 수 있어. 

	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(2, param);	// number of parameter랑 parameter 배열을 넘겨주도록 할거야. 빈상태에서 -> b0, b1을 사용하겠다 계약을 맺고 있는 상태 
	// CD로 시작하는 건 기본적인 DX는 아니고 우리가 받아준 d3dx라이브러리에 포함되어 있는 아이. f12로 살펴보면 기본상태의 설명이 들어가는 걸로 호출이 됨.
	// D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(D3D12_DEFAULT);에서 이런식으로 서명만 살짝 바꿔놨을 뿐인데 많은게 달라지게 될거야. 

	// 아무것도 안쓸것고 땅만 보고 갈게. 성의 없는 사인을 해준거. 다만 flags만 INPUT_ASSEMBLER단계를 사용하겠다고 서명해줬다고 보면 됨. 
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // 입력 조립기 단계
	// 기본 상태로 서명을 해준건데, 다음 시간에는 첫번째 버전으로 추가적인 계약을 하게 될거야. 지금은 아무것도 안함. -> 수정해 줌.

	ComPtr<ID3DBlob> blobSignature;
	ComPtr<ID3DBlob> blobError;
	::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	device->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature));
	// CreateRootSignature를 해주고 있다. 최종적으로는 _signature라는 애한테 최종 정보가 저장이 될거야. 
	// Init만 한 다음에 외부에서 꺼내쓸 때는 GetSignature를 이용해 꺼내쓰면 됨. 
}