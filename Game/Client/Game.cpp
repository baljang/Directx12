#include "pch.h"
#include "Game.h"
#include "Engine.h"

shared_ptr<Mesh> mesh = make_shared<Mesh>(); 
shared_ptr<Shader> shader = make_shared<Shader>(); 

void Game::Init(const WindowInfo& info)
{
//	HelloEngine();  
	GEngine->Init(info);

	// �ﰢ�� �ϳ� ����ž�. 
	vector<Vertex> vec(3);	// 3��¥�� Vertex ���� ���� ������ ä���ָ� ��. 

	vec[0].pos = Vec3(0.f, 0.5f, 0.5f);		// ���� ������, �÷η� ä���ְ� �ִ°� �� �� �־�. 
	vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);	// rgba ��. ��
	vec[1].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[1].color = Vec4(0.f, 1.0f, 0.f, 1.f);	// ��
	vec[2].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);	// ��
	mesh->Init(vec);
	// Init���� �츮�� mesh���� ������� ��ɿ� ���ؼ� GPU�� �ش� ���۸� ����� �޶�� ¡¡�Ÿ��鼭 VIEW�� ������ְ� ��. 
	// Mesh::Render�� ���ٶ��� �Ʊ� ������� �� ���ҽ��� Ȱ���϶�� View�� �ǳ��ִ� �� �� �� �־�. 

	shader->Init(L"..\\Resources\\Shader\\default.hlsli");	// shader�� �̷� ������ Init�� ����.
	// default.hlsli ��ΰ� ���ϰ� �־�. 
	// Client�� ����Ǵ� �Ŵ� C:\Users\Jang\Documents\Directx12\Game\Client �� ��ΰ� ������ ��. 
	// ��ĭ �ڷ� �� ������ Resources�� ���� Shader�� ���� �� ��ġ�� default ������ �ִ� ��. 
	// �׽�Ʈ�� ���� �ϵ��ڵ� �ص� ������ ���߿� ���� ���������� � ��ġ�� ������ �𸣴ϱ� �� Resources�� ������ ��θ� ���� �������� ���� ������ �ִ� ������� ����� �ֱ� �ؾ� ��. 

	GEngine->GetCmdQueue()->WaitSync();	// Ȥ�� ���� ����ȭ�� �ȵ����� ��ٷ� �ش�. 
}

void Game::Update()
{
	//GEngine->Render();

	GEngine->RenderBegin();
	
	shader->Update();	// SetPipelineState�� ���� ������ ���� ������ ���� �־��ְ� �־�. 
	// f12�� �� ���� pipelineState���� ���� ������ ���� ��. �°� ���� �� �� ������ ��������� _pipelineState 
	// �̰� �̿��� �ٽ� �����ϰ� �ִ� �͵� �����ܺ� �κ�
	// �ϰ� ������� ���Ҿ root signature�� ���� ���������� ������ �Ǿ� �ִ� ��.  
	
	{
		Transform t;	// �����ϰ� Transform ����
		t.offset = Vec4(0.75f, 0.f, 0.f, 0.f);	// ���̴��� offset0�� offset1�� �־��� ������ ������ �ɰž�. 
		mesh->SetTransform(t); // ������ ����
		
		mesh->Render();		// ���������� �ﰢ���� �̵��� �Ŷ� ���� ����, ������ ������� �Ŷ� ����
		// vec4�� RGBA�� �м��� ���� �ְ�, ��ǥ�ε� �м��� ���� �־�. ���̴� �ȿ� �ִ� �ڵ尡 �ϱ� ����
	}

	{
		Transform t;	// �����ϰ� Transform ����
		t.offset = Vec4(0.f, 0.75f, 0.f, 0.f);	
		mesh->SetTransform(t); // ������ ����

		mesh->Render();	// �ʷϻ��� �������� �ɰŰ�, ���������� �����ϰ� �Ǹ� ���� �ö󰥰� ���� ����
	}
	// ��ȣ ���� ������ �׳� Transform Ű�� ���� ���� ������ذ�. 

	mesh->Render(); 

	GEngine->RenderEnd();
}
