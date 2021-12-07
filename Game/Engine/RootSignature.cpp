#include "pch.h"
#include "RootSignature.h"

void RootSignature::Init(ComPtr<ID3D12Device> device)
{
	CD3DX12_ROOT_PARAMETER param[2];	
	param[0].InitAsConstantBufferView(0); // 0�� -> b0 -> CBV 
	param[1].InitAsConstantBufferView(1); // 1�� -> b1 -> CBV
	// �׸����� ���� �ʷϻ��� root CBV�� ����ϰڴٰ� ������ ���ִ� �Ű�
	// ���� �׸������� 0,1,2��� �Ǿ� ������ �ʷϻ� 2���� ����� �ٰž�. 0,1�� �� root CBV�� �ɰŰ� 
	// �� ������ ������ ��ȣ�� ������� �Ǵµ� �� �κ��� �־��ְ� �־�. 
	// ()�� �־��� shader register ��ü�� � ��ȣ�� ����� �������� �����ִµ� 0�� ������ b0�� ����ϰڴٴ� ���� 
	// 1���� �־����� b1�� ����ϰڴٴ� �ǹ̾�. ��ġ�� �ȵ�. �ߺ����� �ʰ� ������ ��. 
	// ���� ������� ������ �Ǳ� ������ 0�� ���Կ� b0�� Ȱ���� ���̰� �� �뵵�� CBV�� �̿��Ұ��̴�. �̷� descriptor�� Ȱ���Ұ��̴ٶ�� ������ ���ذ�. 
	// ������ ���ڵ��� ���� �߿����� �ʴµ�, registerSpace ���� ��쿡�� � ������ ���� �̸��� ��ġ�� ������� ������ ��,
	// ���������� �ι�° ������ register space�� �ٸ��� �ϸ� ��׵��� �������� �� �ֱ�� ��. 
	// b0-0, b0-1 �̷� �������� �ѹ��� �и��ϴ� ���������� ���� Ȱ���� ���� �����ž�.  
	// ������ ���ڴ� shader visiblity��� ����. �⺻������ all �� �Ǿ� �ִµ� �� �ʿ��ϴ���. ���� pipeline �׸� ���� ���ʿ��� ���̴� ���·� ���� ��. �ѱ�� ���ڰ� ������ ���ٵ�  
	// �⺻������ Ư�� �ܰ迡���� �ʿ��ϴٰ� �� �Ѱ��� ������, vertexShader������ ���ڴٰ� �ϸ� �ܰ� �Ѿ �� �ҽ��� �ȴ�. 
	// �ٵ� �츮�� visiblity all �� �����ϱ� �Ҹ�Ǵ°� �ƴ϶� ���� ��𼭵� Ȱ��� �� �ְ� ��� �ܰ迡�� �Ⱦ��ְ� �ִ� ���¶�� �� �� �־�. 

	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(2, param);	// number of parameter�� parameter �迭�� �Ѱ��ֵ��� �Ұž�. ����¿��� -> b0, b1�� ����ϰڴ� ����� �ΰ� �ִ� ���� 
	// CD�� �����ϴ� �� �⺻���� DX�� �ƴϰ� �츮�� �޾��� d3dx���̺귯���� ���ԵǾ� �ִ� ����. f12�� ���캸�� �⺻������ ������ ���� �ɷ� ȣ���� ��.
	// D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(D3D12_DEFAULT);���� �̷������� ���� ��¦ �ٲ���� ���ε� ������ �޶����� �ɰž�. 

	// �ƹ��͵� �Ⱦ��Ͱ� ���� ���� ����. ���� ���� ������ ���ذ�. �ٸ� flags�� INPUT_ASSEMBLER�ܰ踦 ����ϰڴٰ� ��������ٰ� ���� ��. 
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // �Է� ������ �ܰ�
	// �⺻ ���·� ������ ���ذǵ�, ���� �ð����� ù��° �������� �߰����� ����� �ϰ� �ɰž�. ������ �ƹ��͵� ����. -> ������ ��.

	ComPtr<ID3DBlob> blobSignature;
	ComPtr<ID3DBlob> blobError;
	::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	device->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature));
	// CreateRootSignature�� ���ְ� �ִ�. ���������δ� _signature��� ������ ���� ������ ������ �ɰž�. 
	// Init�� �� ������ �ܺο��� ������ ���� GetSignature�� �̿��� �������� ��. 
}