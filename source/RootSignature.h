#pragma once
#include <d3d12.h>

class RootSignature
{
public:
	enum class Type { RootConstant, RootConstantBuffer, TableConstantBuffer };

	RootSignature(Type type, UINT entries, UINT depth, bool visible);
	virtual ~RootSignature();

	Type getType() { return type; }
	ID3D12RootSignature * getPtr() { return rootSign; }

private:
	ID3D12RootSignature * rootSign;
	Type type;

	D3D12_DESCRIPTOR_RANGE createDescRange(UINT entries);
	D3D12_ROOT_DESCRIPTOR_TABLE createDescTable(D3D12_DESCRIPTOR_RANGE * ptr, UINT num);
	D3D12_ROOT_PARAMETER createRootParam(D3D12_ROOT_PARAMETER_TYPE type);
	
};
