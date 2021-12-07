#pragma once

// [�ϰ� �����] ���� �ηµ��� �� �ؾ����� ��� 
class Shader
{
public:
	void Init(const wstring& path);
	// �ܺο� �ִ� �ٸ� ���Ϸ� ������ �ϰ� �ɰž�. �װ� �ҷ��� �ε带 �� ������ �¸� ����� �༭ �����ϰ� �ɰž�. 
	void Update();

private:
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);

private:
	ComPtr<ID3DBlob>					_vsBlob;
	ComPtr<ID3DBlob>					_psBlob;
	ComPtr<ID3DBlob>					_errBlob;
	// ������ ������� �ϴ� �����ϰ� ��׵��� ���ҽ��� �̷��� �޾��ִ±��� ��� ���¿� �̷� ���ڵ�� ���� ����� �ִ±��� �ϰ� �Ѱ��ָ� ��. 

	ComPtr<ID3D12PipelineState>			_pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _pipelineDesc = {};

};

