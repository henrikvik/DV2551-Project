#pragma once
#include <d3d12.h>

#define GET(var) inline auto get_##var##() const { return var; };

class RootSignature
{
public:
	enum class Type { RootConstant, RootConstantBuffer, TableConstantBuffer };
	enum class Visiblity { 
		All		= D3D12_SHADER_VISIBILITY_ALL, 
		Vertex	= D3D12_SHADER_VISIBILITY_VERTEX,
		Pixel	= D3D12_SHADER_VISIBILITY_PIXEL
	};

	RootSignature(Type type, UINT num, Visiblity visibilty);
	virtual ~RootSignature();

	inline auto get_type()		 const { return this->type;		 }
	inline auto get_num()		 const { return this->num;		 }
	inline auto get_visibility() const { return this->visibilty; }
	inline ID3D12RootSignature * get_ptr()   { return this->ptr; }

private:
	Type type;
	UINT num;
	Visiblity visibilty;
	ID3D12RootSignature * ptr;
	
};
