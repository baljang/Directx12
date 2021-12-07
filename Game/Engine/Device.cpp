#include "pch.h"
#include "Device.h"

void Device::Init()
{
// D3D12 디버그층 활성화
// - VC++ 출력창에 상세한 디버깅 메시지 출력
// - riid : 디바이스의 COM ID
// - ppDevice : 생성된 장치가 매개변수에 설정
#ifdef _DEBUG	// 디버그 용도로만 활용하는 기능. 추가적인 정보를 디버그 할 때 준다. 
	::D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController));
	_debugController->EnableDebugLayer();
	// IID_PPV_ARGS를 f12로 들어가서 보면 넘겨준 아이의 아이디를 출력을하고, 이어서 뭔가를 작업해줌을 알 수 있어. 
	// 한마디로 아이디랑 포인터를 어떻게든 꺼내준다는 것을 알 수 있어. IID_PPV_ARGS매크로를 이용해 편리하게 하는거. 
#endif

	// DXGI(DirectX Graphics Infrastructure)
	// Direct3D와 함께 쓰이는 API
	// - 전체 화면 모드 전환
	// - 지원되는 디스플레이 모드 열거 등
	// CreateDXGIFactory
	// - riid : 디바이스의 COM ID
	// - ppDevice : 생성된 장치가 매개변수에 설정
	::CreateDXGIFactory(IID_PPV_ARGS(&_dxgi));
	// 사실 우리가 다이렉트 엑스를 활용한다고 하되 모든 기능들이 다 device에 들어있는 건 아냐. 일반적인 화면 디스플레이와 관련된 기능들은 dxgi에 들어가 있어. 
	// 나중에 사용할 swapchain이런 기능들도 dxgi를 통해 만든다고 볼 수 있어. 
	// 어떻게 보면 device와 dxgi가 양대 산맥이라고 볼 수 있어. 인력사무소의 대표라고 볼 수 있어. 

	// CreateDevice
	// - 디스플레이 어댑터(그래픽 카드)를 나타내는 객체
	// - pAdapter : nullptr 지정하면 시스템 기본 디스플레이 어댑터		<- 각각의 인자들은 책을 보면서 추가 정보를 보고 싶다면 보면 됨. 
	// - MinimumFeatureLevel : 응용 프로그램이 요구하는 최소 기능 수준 (구닥다리 걸러낸다) 
	// - riid : 디바이스의 COM ID		// IID_PPV_ARGS()에서 COM 객체마다 각각의 ID가 있다고 보면 되는데 ID를 꺼내주고 
	// - ppDevice : 생성된 장치가 매개변수에 설정		// 생성된 장치의 2중 포인터를 넘겨주는 것 까지는 동일하고 
	::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device));
	// device라는 애는 인력 사무소의 대장님이라고 볼 수 있어. 
	// 그래픽 카드에 어떤 기능을 요청할 때 이 device라는 애를 통해서 접근을 해서 뭔가를 요청하게 됨. 
	// D3D_FEATURE_LEVEL_11_0 이부분이 흥미로운데 응용프로그램이 요구하는 최소 기능 수준을 입력을 해줄 수 있어. 
	// 만약 다이렉트9 까지만 제공하는 그래픽카드면 D3D_FEATURE_LEVEL_9_1를 입력하면 되고, 우리는 최소 11를 지워하는 그래픽카드라고 쓴거. 
	
	// 이렇게 간단하게 device 하나를 만들어줬고 나머지도 이렇게 한땀한땀 만들어주게 될거야. 
	

}
