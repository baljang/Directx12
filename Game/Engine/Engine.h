#pragma once

#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "Mesh.h"
#include "Shader.h"
#include "ConstantBuffer.h"		// �̷������� ����� ���� �߰��Ǵ°� �ٶ������� ������ ������ DX ��ü �����ϴ� �Ŵϱ� �����ϰ� ������. 

class Engine
{

public: 
	void Init(const WindowInfo& info);	// �ʱ�ȭ
				// EnginePch.h���� ����� ��� struct ���ڷ� �־���. 
	void Render();	// �׷��ִ� 

public: 
	shared_ptr<Device> GetDevice() { return _device; }
	shared_ptr<CommandQueue> GetCmdQueue() { return _cmdQueue; }
	shared_ptr<SwapChain> GetSwapChain() { return  _swapChain; }
	shared_ptr<RootSignature> GetRootSignature() { return _rootSignature; }
	shared_ptr<ConstantBuffer> GetCB() { return _cb; }

public:
	void RenderBegin();
	void RenderEnd();
	// End��� �ؼ� ������ �ƴ϶� CommandQueue���ٰ� �̷����� ��û���׵��� �� �־��ִ°� RenderBegin�̰�
	// RenderEnd�� �׾� ���� �ϰ����� �� �ذ����� GPU���� ���ַ� �ðܼ� ���������� ��� �ϰ����� �׸��� �۾��� ����Ǵ� �κ�. �̸��� �ٲ㵵 ��. 

	// �������� ������ �޾����ڸ���, �������� ũ�⸦ ������ �ϴ� �Լ�
	void ResizeWindow(int32 width, int32 height);
	
private: 
	// �׷��� ȭ�� ũ�� ����
	WindowInfo _window; 
	D3D12_VIEWPORT _viewport = {};
	D3D12_RECT _scissorRect = {};
	// _viewport�� _scissorRect��� �ִ� GPU�����ε� �˷��ֱ� �ؾ��ϴ� �κе�
	
	// �ٽ��� �߰��� Ŭ���� 4�ѻ�
	shared_ptr<Device> _device; 
	shared_ptr<CommandQueue> _cmdQueue; 
	shared_ptr<SwapChain> _swapChain; 
	// �ʱ�ȭ�� �ϴµ� �� �ʿ��� �κе�. ���� � ������ �ϴ����� Ŭ���� �����ϸ鼭 ������. 
	shared_ptr<RootSignature> _rootSignature;
	shared_ptr<ConstantBuffer> _cb; 
};

