#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <crtdbg.h>
#include <comdef.h>
#

#define FRAME_COUNT 2

#define SafeRelease(ptr) {if(ptr){ptr->Release();ptr=nullptr;}}
#define SafeDelete(ptr) {if(ptr){delete ptr;ptr=nullptr;}}


#define BreakOnFail(hr)  {\
    if FAILED(hr) \
    { \
        OutputDebugString(_bstr_t("\n---==+==---\n"));\
        OutputDebugString(_com_error(hr).ErrorMessage());\
        OutputDebugString(_bstr_t("\n---==+==---\n"));\
        _CrtDbgBreak();\
    }\
}

#define BreakOnFail1(hr, message)\
{\
    if FAILED(hr)\
    {\
        OutputDebugString(_bstr_t("\n---==+==---\n"));\
        OutputDebugString(_bstr_t(message));\
        OutputDebugString(_bstr_t("\n---==+==---\n"));\
        _CrtDbgBreak();\
    }\
}

#define BreakOnFail2(hr, blob)\
{\
    if FAILED(hr)\
    {\
        OutputDebugString(_bstr_t("\n---==+==---\n"));\
        OutputDebugString(_bstr_t((char*)blob->GetBufferPointer()));\
        OutputDebugString(_bstr_t("\n---==+==---\n"));\
        _CrtDbgBreak();\
    }\
}

// Used to toggle a flag off on easily
#define TOGGLE_FLAG(dest, flag) (dest & flag) ? dest &= ~flag : dest |= flag

// Used to check if a flag is toggled of or not
#define CHECK_FLAG(dest, flag) dest & flag

struct G
{
	ID3D12Device*               device = nullptr;
    
    IDXGISwapChain3*            swap_chain = nullptr;

    // Render Targets
    ID3D12Resource*             render_target[FRAME_COUNT];
    ID3D12DescriptorHeap*       render_target_heap;

    // Command Resources
    ID3D12CommandAllocator*     command_allocator = nullptr;
    ID3D12CommandQueue*         command_queue = nullptr;
    ID3D12GraphicsCommandList*  command_list = nullptr;

    // Fence Resources
    ID3D12Fence*                fence = nullptr;
    UINT                        frame_index = 0;
    HANDLE                      fence_event;
    UINT64                      fence_value = 0;

    // RS
    CD3DX12_VIEWPORT            view_port;
    CD3DX12_RECT                scissor_rect;
};

extern G g;