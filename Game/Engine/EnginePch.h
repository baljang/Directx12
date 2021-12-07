#pragma once

// 각종 include
#include <windows.h>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
using namespace std;

#include "d3dx12.h"		// 마이크로소프트에서 만든거긴 한데 비공식적으로 딸려오는 라이브러리라서 따로 추가해줘야 함. 
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

// 각종 lib
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

// 각종 typedef
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
	HWND	hwnd; // 출력 윈도우
	int32	width; // 너비
	int32	height; // 높이
	bool	windowed; // 창모드 or 전체화면
};

struct Vertex
{
	Vec3 pos;	// Vec3는 float가 3개 있는거. x,y,z 3차원 공간이라
	Vec4 color; // Vec4는 float가 4개 있는거. r,g,b,a 4개의 값이라 
};

struct Transform
{
	Vec4 offset; 
};

#define DEVICE		GEngine->GetDevice()->GetDevice()
#define CMD_LIST		GEngine->GetCmdQueue()->GetCmdList()
#define ROOT_SIGNATURE	GEngine->GetRootSignature()->GetSignature()

// class Engine; 이렇게 따로 빼줘도 되는데 아래처럼 class만 적어줘도 된다. 
extern unique_ptr<class Engine> GEngine;	// GEngine이라는 애가 등장할거야 선포
// extern : 다른 파일에서 이미 이름이 같은 전역변수가 선언이 되었다는 의미. 즉, 다른 파일간의 변수를 공유하고 있다라는 뜻이다.

// void HelloEngine(); 더이상 활용 안함
