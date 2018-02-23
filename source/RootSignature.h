#pragma once
#include <d3d12.h>

class RootSignature
{
public:
	RootSignature(bool buffer, unsigned int entries);
	virtual ~RootSignature();

	ID3D12RootSignature * get() { return rootSign; }
private:
	ID3D12RootSignature * rootSign;
};