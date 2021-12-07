#pragma once

// [계약서 / 결재] 

// CPU [		]	GPU [		]

// 계약서가 왜 필요하느냐, 외주의 주체인 CPU가 할 것을 GPU한테 외주를 주고 있는 개념. 
// 놀랍게도 둘이 사용하는 공간이 완전히 다르다. 
// CPU에서 사용하는 것을 시스템 메모리라고 해. 내부에 있는 건 아니고 ram에서 뭔가를 데이터를 저장해서 그 메모리에 있는걸 꺼내서 CPU가 사용하고 
// 이런식으로 ram이라 CPU랑 친밀한 관계인데 GPU에 외주를 맞길때도 GPU도 어떤 데이터를 들고 있기는 해야 해. 그런데 GPU같은 경우는 ram에 있는 꺼내 왔다 갔다 하는건 말이 안돼. 
// 너무 멀리 있어서. 그래서 외주를 맡기고 싶을 때는 GPU 쪽에다가 데이터를 어떤 식으로든 전달을 한 다음에 거기에 있는 데이터를 토대로 GPU가 외주를 맡기를 시작해야 해. 

// 한국 [		]	베트남[		] 
// 한국에서 발주를 하고 베트남에 외주 인력들이 있다고 가정해 보자. 
// 외주를 맡기고 싶으면 외주할 일이 어떤 걸지를 명확하게 이야기 해야 해. 
// 얘기를 하는 것 뿐만 아니라 무엇을 대상으로 가공을 해야 할지도 얘기를 해줘야해. 
// 예를 들어 원석을 미리 준비한 다음에 원석을 가공하는 임무를 맡기고 싶다면 당연하지만 그 원석을 베트남에 보내가지고 베트남에서 가공을 해야 해. 
// 결국에는 계약서라는게 베트남[		] 여기에도 공간들이 있는데 거기를 임대를 해서 사용을 하겠다 계약을 하는 것. 
// 즉 어떠한 형태로든 이런 저런 정보들을 반대쪽에 건내준 다음에 그걸 활용을 해야하는데 그런 일련의 과정들이 멋대로 이루어 지는게 아니라 확실하게 설명을 한다.
// RootSignature를 이용해 설명을 해가지고 내가 어떠어떠한 레지스터, 버퍼를 활용을 하겠다 명시를 해주는 것. 말 그대로 어떤 정책을 사용할 것인지, 어떤 땅을 임대할 것인지 계약서를 쓰는 느낌. 
// 자세한 내용 GPU 메모리에 관련된 내용들은 다음 시간에 좀 더 깊게 다뤄보도록 할거야. 지금 단계에서는 왠지는 모르겠지만 계약서를 쓴다고 하니까 멋도 모르고 도장을 찍으면 되는 거. 
class RootSignature
{
public: 
	void Init(ComPtr<ID3D12Device> device);

	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

private: 
	ComPtr<ID3D12RootSignature> _signature;		// RootSignature라는 걸 만들어 줬어. ComPtr로 되어있는 거의 대부분의 것들을 GPU에게 요청을 하는 거. 거기 내부에서 뭔가 실질적 객체가 만들어진다고 보면 됨.
												// GPU쪽에다 어떤 계약서를 건내주는 느낌이라고 보면 됨. 

};

