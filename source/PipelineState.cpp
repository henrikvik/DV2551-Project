#include "PipelineState.h"
#include "RootSignature.h"
#include "Helper.h"
#include <comdef.h>

#pragma comment(lib,"D3DCompiler.lib")

PipelineState::PipelineState(RootSignature* rootSignature) 
{
    ID3DBlob* vsShader;
    ID3DBlob* psShader;
    ID3DBlob* errorMsg;

#if defined(_DEBUG)
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif

    switch (rootSignature->get_type())
    {
    case RootSignature::Type::RootConstant: 
        break;
    case RootSignature::Type::RootConstantBuffer: 
        break;
    case RootSignature::Type::TableConstantBuffer: 
        break;
    }

    BreakOnFail2(D3DCompileFromFile(L"Shader.hlsl", nullptr, nullptr, "VSMain", "vs_5_1", compileFlags, 0, &vsShader, &errorMsg), errorMsg);
    BreakOnFail2(D3DCompileFromFile(L"Shader.hlsl", nullptr, nullptr, "PSMain", "ps_5_1", compileFlags, 0, &psShader, &errorMsg), errorMsg);

    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc = {};
    pipelineStateDesc.pRootSignature = rootSignature->get_ptr();
    pipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(vsShader);
    pipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(psShader);
    pipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    pipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    pipelineStateDesc.DepthStencilState.DepthEnable = FALSE;
    pipelineStateDesc.DepthStencilState.StencilEnable = FALSE;
    pipelineStateDesc.SampleMask = UINT_MAX;
    pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pipelineStateDesc.NumRenderTargets = 1;
    pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    pipelineStateDesc.SampleDesc.Count = 1;

    g.device->CreateGraphicsPipelineState(&pipelineStateDesc, IID_PPV_ARGS(&pipelineState));
}

PipelineState::~PipelineState()
{
    SafeRelease(pipelineState);
}