#include "RootSignature.h"
#include "Helper.h"

RootSignature::RootSignature(Type type, UINT num, Visiblity visibilty)
{
	D3D12_ROOT_PARAMETER rp			= {};
	D3D12_DESCRIPTOR_RANGE dr		= {};
	D3D12_ROOT_PARAMETER * rps		= nullptr;

	D3D12_ROOT_SIGNATURE_DESC rsd = {};
	rsd.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

	if (type == Type::RootConstant)
	{
		rps = new D3D12_ROOT_PARAMETER[num];
		for (size_t i = 0; i < num; i++)
		{
			D3D12_ROOT_CONSTANTS rc = {};
			rc.Num32BitValues = 1;
			rc.RegisterSpace = i;
			rc.ShaderRegister = 0;

			rps[i].ShaderVisibility = (D3D12_SHADER_VISIBILITY)visibilty;
			rps[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
			rps[i].Constants = rc;
		}

		rsd.NumParameters = num;
		rsd.pParameters   = rps;
	}
	else if (type == Type::RootConstantBuffer)
	{
		rps = new D3D12_ROOT_PARAMETER[num];
		for (size_t i = 0; i < num; i++)
		{
			rps[i].ShaderVisibility			 = (D3D12_SHADER_VISIBILITY)visibilty;
			rps[i].ParameterType		     = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rps[i].Descriptor.RegisterSpace  = i;
			rps[i].Descriptor.ShaderRegister = 0;
		}

		rsd.NumParameters = num;
		rsd.pParameters   = rps;
	}
	else if (type == Type::TableConstantBuffer)
	{
		dr.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		dr.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		dr.BaseShaderRegister = 0;
		dr.RegisterSpace = 0;
		dr.NumDescriptors = num;

		D3D12_ROOT_DESCRIPTOR_TABLE rdt = {};
		rdt.NumDescriptorRanges = 1;
		rdt.pDescriptorRanges = &dr;

		rp.ShaderVisibility = (D3D12_SHADER_VISIBILITY)visibilty;
		rp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rp.DescriptorTable = rdt;

		rsd.NumParameters = 1;
		rsd.pParameters = &rp;
	}


	ID3DBlob *blob, *err;
	BreakOnFail2(D3D12SerializeRootSignature(
		&rsd,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&blob,
		&err
	), err);


	if (type == Type::RootConstantBuffer) delete[] rps;
}

RootSignature::~RootSignature()
{
	SafeRelease(ptr);
}
