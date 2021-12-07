#pragma once

// 인력 사무소
class Device
{
	// GPU와 핑퐁을 할 그 역할을 담당하는 사무소라고 보면 됨. 
	// 각종 객체를 생성하거나 하는 부분을 담당
	// Directx9에서는 Device라는 클래스에서 거의 모든 기능을 올인원으로 담당하고 있었는데 
	// 버전 올라가면서 기능들이 이리저리 쪼개졌어. 그래서 우리도 쪼개 놓은 거 
public: 
	void Init();	// 초기화 하는 함수 

	ComPtr<IDXGIFactory> GetDXGI() { return _dxgi; }		// 외부에서 꺼내쓸 수 있도록
	ComPtr<ID3D12Device> GetDevice() { return _device; }

private: 
	// ComPtr이라는 묘한 애가 등장을 했는데 DirectX에 대해 다시 요약을 해보면 
	// DirectX자체는 GPU를 제어하고 프로그래밍 하는데 쓰이는 저수준 그래픽 API라는 특징이 있어. 
	// 우리는 DirectX만 신경쓰고 MS와 GPU 제조사들이 협력을 해가지고 DirectX 표준에 따라 제조를 한다. 
	// 그걸 가능하게 하는 기술 중 하나가 COM

	// COM(Component Object Model)
	// - DX의 프로그래밍 언어 독립성과 하위 호환성을 가능하게 하는 기술
	// - COM 객체(COM 인터페이스)를 사용. 세부사항은 우리한테 숨겨짐 
	//	 : GPU의 세부적인 스펙에 대해선 알지 못하지만 만들어준 COM 객체들을 이용해 GPU에 접근을 하고 뭔가를 하는 마냥 작동을 하게 할 수 있음.
	// - ComPtr 일종의 스마트 포인터
	//   : Com 객체들을 받아주는. f12로 들어가보면 뭔가 레퍼런스를 관리하고 있음.
	//	   Com 객체를 사용할 때는 ComPtr을 1+1으로 사용할건데 그렇게 되면 일반적 C++처럼 new와 delete로 동적할당하는 개념이 아니라 그냥 만들어 준 다음 스마트포인터처럼 잊고 살아도 됨. 
	//     언젠가는 reference count 가 0이 되어서 아무도 기억 못할 때 알아서 삭제가 될거야. 
	//     <>안에 인터페이스들이 하나씩 들어가 있는데 일반적인 클래스라고 가정하고 똑같이 만들어도 됨. 
	ComPtr<ID3D12Debug>		_debugController; // 디버그 용도 기능
	ComPtr<IDXGIFactory>	_dxgi; // 화면 관련 기능들
	ComPtr<ID3D12Device>	_device; // 각종 객체 생성
	// 아직 딱히 뭘 하는지는 모르겠지만 일종의 스마트 포인터로 이러한 COM 객체들을 관리하고 있다는 걸 알면 됨.
	// 이제 이 3총사를 인력 사무소에 배치해서 만들어 볼거야. 



};

