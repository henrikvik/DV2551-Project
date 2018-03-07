#ifndef PIPELINESTATE_H
#define PIPELINESTATE_H

#include <d3d12.h>
#include <D3dx12.h>
#include <D3Dcompiler.h>

class RootSignature;
class PipelineState
{
public:
    PipelineState(RootSignature* rootSignature);
    ~PipelineState();

    ID3D12PipelineState* getPipelineState() { return pipelineState; }
    operator ID3D12PipelineState*() { return pipelineState; }
private:
    ID3D12PipelineState* pipelineState;
};


#endif // !PIPELINESTATE_H
