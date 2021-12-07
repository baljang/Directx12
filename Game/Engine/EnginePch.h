#pragma once

// ���� include
#include <windows.h>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
using namespace std;

#include "d3dx12.h"		// ����ũ�μ���Ʈ���� ����ű� �ѵ� ����������� �������� ���̺귯���� ���� �߰������ ��. 
#include <d3d12.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

// ���� lib
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

// ���� typedef
using int8		= __int8;
using int16		= __int16;
using int32		= __int32;
using int64		= __int64;
using uint8		= unsigned __int8;
using uint16	= unsigned __int16;
using uint32	= unsigned __int32;
using uint64	= unsigned __int64;
using Vec2		= XMFLOAT2;
using Vec3		= XMFLOAT3;
using Vec4		= XMFLOAT4;
using Matrix	= XMMATRIX;

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2,
};

struct WindowInfo
{
	HWND	hwnd; // ��� ������
	int32	width; // �ʺ�
	int32	height; // ����
	bool	windowed; // â��� or ��üȭ��
};

struct Vertex
{
	Vec3 pos;	// Vec3�� float�� 3�� �ִ°�. x,y,z 3���� �����̶�
	Vec4 color; // Vec4�� float�� 4�� �ִ°�. r,g,b,a 4���� ���̶� 
};

struct Transform
{
	Vec4 offset; 
};

#define DEVICE		GEngine->GetDevice()->GetDevice()
#define CMD_LIST		GEngine->GetCmdQueue()->GetCmdList()
#define ROOT_SIGNATURE	GEngine->GetRootSignature()->GetSignature()

// class Engine; �̷��� ���� ���൵ �Ǵµ� �Ʒ�ó�� class�� �����൵ �ȴ�. 
extern unique_ptr<class Engine> GEngine;	// GEngine�̶�� �ְ� �����Ұž� ����
// extern : �ٸ� ���Ͽ��� �̹� �̸��� ���� ���������� ������ �Ǿ��ٴ� �ǹ�. ��, �ٸ� ���ϰ��� ������ �����ϰ� �ִٶ�� ���̴�.

// void HelloEngine(); ���̻� Ȱ�� ����
