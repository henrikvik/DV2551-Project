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
		shader_code << "cbuffer cb" << i << " : register(b" << i << ") { float v" << i << "; }\n";
	}

	std::string mul = "v0";
	for (size_t i = 1; i < num; i++)
	{
		mul += " * v" + std::to_string(i);
	}

	shader_code << "float4 VS_MAIN() : SV_POSITION" << std::endl;
	shader_code << "{"								<< std::endl;
	shader_code << "    float v = " << mul << ";"	<< std::endl;
	shader_code << "    return float4(v,v,v,v);"	<< std::endl;
	shader_code << "}"								<< std::endl;

	shader_code << "float4 PS_MAIN(float pos : SV_POSITION) : SV_TARGET0" << std::endl;
	shader_code << "{"													  << std::endl;
	shader_code << "    return pos;"									  << std::endl;
	shader_code << "}"													  << std::endl;

	/*/
	std::ofstream fout("dump.hlsl");
	fout << shader_code.str();
	fout.close();//*/

	std::string code = shader_code.str();

	ID3DBlob *err;
	BreakOnFail2(D3DCompile(
		code.c_str(),
		code.length(),
		nullptr,
		nullptr,
		nullptr,
		"VS_MAIN",
		"vs_5_1",
		0,
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
		"PS_MAIN",
		"ps_5_1",
		0,
		0,
		&ps,
		&err
	), err);

}

Shader::~Shader()
{
}
