#include "Shader.h"
#include <string>
#include <sstream>
#include <fstream>
#include <d3dcompiler.h>
#include "Helper.h"


Shader::Shader(UINT num)
{
	std::stringstream shader_code;

	for (size_t i = 0; i < num; i++)
	{
		shader_code << "cbuffer cb" << i << " : register(b" << i << ") { float v" << i << " = 0; }\n";
	}

	std::string mul = "sqrt(v0)";
	for (size_t i = 1; i < num; i++)
	{
		mul = mul + "* v" + std::to_string(i);
	}


	shader_code << "struct Pixel"				   << std::endl;
	shader_code << "{"							   << std::endl;
	shader_code << "    float4 pos : SV_POSITION;" << std::endl;
	shader_code << "};"							   << std::endl;

	shader_code << "Pixel vs_main()"				 << std::endl;
	shader_code << "{"								 << std::endl;
	shader_code << "    Pixel p;"					 << std::endl;
	shader_code << "    float v = 1 / (" << mul << ");" << std::endl;
	shader_code << "    p.pos = float4(v, 0, 0, 1);" << std::endl;
	shader_code << "    return p;"					 << std::endl;
	shader_code << "}"								 << std::endl;

	shader_code << "float4 ps_main(Pixel p) : SV_TARGET0" << std::endl;
	shader_code << "{"									  << std::endl;
	shader_code << "    return p.pos;"					  << std::endl;
	shader_code << "}"									  << std::endl;

	std::string code = shader_code.str();
	/*/
	std::ofstream fout("dump.hlsl");
	fout << code;
	fout.close();//*/


	ID3DBlob *err;
	BreakOnFail2(D3DCompile(
		code.c_str(),
		code.length(),
		nullptr,
		nullptr,
		nullptr,
		"vs_main",
		"vs_5_1",
        D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vs,
		&err
	), err);

	BreakOnFail2(D3DCompile(
		code.c_str(),
		code.length(),
		nullptr,
		nullptr,
		nullptr,
		"ps_main",
		"ps_5_1",
		0,
		0,
		&ps,
		&err
	), err);

	SafeRelease(err);
}

Shader::~Shader()
{
	SafeRelease(vs);
	SafeRelease(ps);
}
