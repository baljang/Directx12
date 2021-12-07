#pragma once

// [일감 기술서] 외주 인력들이 뭘 해야할지 기술 
class Shader
{
public:
	void Init(const wstring& path);
	// 외부에 있는 다른 파일로 관리를 하게 될거야. 그걸 불러서 로드를 한 다음에 걔를 만들어 줘서 실행하게 될거야. 
	void Update();

private:
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);

private:
	ComPtr<ID3DBlob>					_vsBlob;
	ComPtr<ID3DBlob>					_psBlob;
	ComPtr<ID3DBlob>					_errBlob;
	// 세세한 내용들은 일단 무시하고 얘네들이 리소스를 이렇게 받아주는구나 라는 상태와 이런 인자들로 뭔가 만들어 주는구나 하고 넘겨주면 됨. 

	ComPtr<ID3D12PipelineState>			_pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _pipelineDesc = {};

};

