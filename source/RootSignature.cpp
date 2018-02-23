#include "RootSignature.h"

RootSignature::RootSignature(Type type, UINT entries, UINT depth, bool visible)
{
	D3D12_ROOT_DESCRIPTOR rd;
	D3D12_ROOT_CONSTANTS rc;
	rc.Num32BitValues = 32;
	rc.RegisterSpace = 0;
	rc.ShaderRegister = 0;
}

RootSignature::~RootSignature()
{
}


D3D12_DESCRIPTOR_RANGE RootSignature::createDescRange(UINT entries)
{
	D3D12_DESCRIPTOR_RANGE dr = {};
	dr.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	dr.RegisterSpace = 0;
	dr.BaseShaderRegister = 0;
	dr.NumDescriptors = entries;
	dr.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	return dr;
}

D3D12_ROOT_DESCRIPTOR_TABLE RootSignature::createDescTable(D3D12_DESCRIPTOR_RANGE * ptr, UINT num)
{
	D3D12_ROOT_DESCRIPTOR_TABLE rdt;
	rdt.pDescriptorRanges = ptr;
	rdt.NumDescriptorRanges = num;
	return rdt;
}

D3D12_ROOT_PARAMETER RootSignature::createRootParam(D3D12_ROOT_PARAMETER_TYPE type)
{
	D3D12_ROOT_PARAMETER rp;
	rp.ParameterType = type;
	return rp;
}
