#pragma once
#include <d3d12.h>
#

#define SafeRelease(ptr) {if(ptr){ptr->Release();ptr=nullptr;}}


struct G
{
	ID3D12Device * device;
};

extern G g;