#ifndef PIPELINESTATE_H
#define PIPELINESTATE_H

#include <d3d12.h>
#include <D3dx12.h>
#include <D3Dcompiler.h>

class PipelineState
{
public:
    PipelineState(ID3D12RootSignature* rootSignature);
    ~PipelineState();

    ID3D12PipelineState* getPipelineState() { return pipelineState; }

private:
    ID3D12PipelineState* pipelineState;
};


#endif // !PIPELINESTATE_H
