#include "pch.h"
#include "Game.h"
#include "Engine.h"

shared_ptr<Mesh> mesh = make_shared<Mesh>(); 
shared_ptr<Shader> shader = make_shared<Shader>(); 

void Game::Init(const WindowInfo& info)
{
//	HelloEngine();  
	GEngine->Init(info);

	// 삼각형 하나 만들거야. 
	vector<Vertex> vec(3);	// 3개짜리 Vertex 벡터 만들어서 각각을 채워주면 됨. 

	vec[0].pos = Vec3(0.f, 0.5f, 0.5f);		// 각각 포지션, 컬로로 채워주고 있는거 볼 수 있어. 
	vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);	// rgba 값. 빨
	vec[1].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[1].color = Vec4(0.f, 1.0f, 0.f, 1.f);	// 초
	vec[2].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);	// 파
	mesh->Init(vec);
	// Init에서 우리가 mesh에서 만들어준 기능에 의해서 GPU에 해당 버퍼를 만들어 달라고 징징거리면서 VIEW도 만들어주게 됨. 
	// Mesh::Render를 해줄때는 아까 만들어준 그 리소스를 활용하라고 View를 건내주는 걸 볼 수 있어. 

	shader->Init(L"..\\Resources\\Shader\\default.hlsli");	// shader도 이런 식으로 Init을 해줘.
	// default.hlsli 경로가 묘하게 있어. 
	// Client가 실행되는 거니 C:\Users\Jang\Documents\Directx12\Game\Client 이 경로가 잡히게 됨. 
	// 한칸 뒤로 간 다음에 Resources에 가서 Shader에 가면 그 위치에 default 파일이 있는 거. 
	// 테스트할 때는 하드코딩 해도 되지만 나중에 가면 실행파일이 어떤 위치에 있을지 모르니까 이 Resources의 폴더의 경로를 따로 설정값을 통해 전달해 주는 방식으로 만들어 주긴 해야 함. 

	GEngine->GetCmdQueue()->WaitSync();	// 혹시 뭔가 동기화가 안됐으면 기다려 준다. 
}

void Game::Update()
{
	//GEngine->Render();

	GEngine->RenderBegin();
	
	shader->Update();	// SetPipelineState를 통해 무엇을 할지 설정한 것을 넣어주고 있어. 
	// f12로 들어가 보면 pipelineState에도 많은 정보가 들어가게 됨. 온갖 정보 다 들어간 다음에 만들어진게 _pipelineState 
	// 이걸 이용해 다시 설정하고 있는 것도 눈여겨볼 부분
	// 일감 기술서와 더불어서 root signature도 같이 간접적으로 포함이 되어 있는 거.  
	
	{
		Transform t;	// 간단하게 Transform 세팅
		t.offset = Vec4(0.75f, 0.f, 0.f, 0.f);	// 셰이더의 offset0과 offset1이 넣어준 값으로 세팅이 될거야. 
		mesh->SetTransform(t); // 데이터 저장
		
		mesh->Render();		// 오른쪽으로 삼각형이 이동할 거라 예측 가능, 빨간색 도드라질 거라 예측
		// vec4가 RGBA로 분석할 수도 있고, 좌표로도 분석할 수도 있어. 셰이더 안에 있는 코드가 하기 나음
	}

	{
		Transform t;	// 간단하게 Transform 세팅
		t.offset = Vec4(0.f, 0.75f, 0.f, 0.f);	
		mesh->SetTransform(t); // 데이터 저장

		mesh->Render();	// 초록색이 진해지게 될거고, 포지션으로 생각하게 되면 위로 올라갈거 예측 가능
	}
	// 괄호 해준 이유는 그냥 Transform 키를 재사용 위해 만들어준거. 

	mesh->Render(); 

	GEngine->RenderEnd();
}
