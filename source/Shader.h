#pragma once
#include <d3d12.h>

class Shader
{
public:
	Shader(UINT num_buffers);
	~Shader();

	inline auto get_vs() const { return vs; }
	inline auto get_ps() const { return ps; }
private:
	ID3DBlob * vs;
	ID3DBlob * ps;
};

