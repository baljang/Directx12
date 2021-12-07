#pragma once

#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "Mesh.h"
#include "Shader.h"
#include "ConstantBuffer.h"		// 이런식으로 헤더가 많이 추가되는게 바람직하진 않지만 지금은 DX 자체 공부하는 거니까 간단하게 만들어본거. 

class Engine
{

public: 
	void Init(const WindowInfo& info);	// 초기화
				// EnginePch.h에서 만들어 줬던 struct 인자로 넣어줌. 
	void Render();	// 그려주는 

public: 
	shared_ptr<Device> GetDevice() { return _device; }
	shared_ptr<CommandQueue> GetCmdQueue() { return _cmdQueue; }
	shared_ptr<SwapChain> GetSwapChain() { return  _swapChain; }
	shared_ptr<RootSignature> GetRootSignature() { return _rootSignature; }
	shared_ptr<ConstantBuffer> GetCB() { return _cb; }

public:
	void RenderBegin();
	void RenderEnd();
	// End라고 해서 끝난게 아니라 CommandQueue에다가 이런저런 요청사항들을 막 넣어주는게 RenderBegin이고
	// RenderEnd는 쌓아 놓은 일감들을 뿅 해가지고 GPU에게 외주로 맡겨서 실질적으로 모든 일감들을 그리는 작업이 실행되는 부분. 이름은 바꿔도 됨. 

	// 윈도우의 정보를 받아주자마자, 윈도우의 크기를 변경을 하는 함수
	void ResizeWindow(int32 width, int32 height);
	
private: 
	// 그려질 화면 크기 관련
	WindowInfo _window; 
	D3D12_VIEWPORT _viewport = {};
	D3D12_RECT _scissorRect = {};
	// _viewport와 _scissorRect라는 애는 GPU쪽으로도 알려주긴 해야하는 부분들
	
	// 핵심은 추가한 클래스 4총사
	shared_ptr<Device> _device; 
	shared_ptr<CommandQueue> _cmdQueue; 
	shared_ptr<SwapChain> _swapChain; 
	// 초기화를 하는데 꼭 필요한 부분들. 각자 어떤 역할을 하는지는 클래스 구현하면서 만들어볼것. 
	shared_ptr<RootSignature> _rootSignature;
	shared_ptr<ConstantBuffer> _cb; 
};

