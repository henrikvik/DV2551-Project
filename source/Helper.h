#pragma once
#include <d3d12.h>
#include <crtdbg.h>
#include <comdef.h>
#

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

// Used to toggle a flag off/on easily
#define TOGGLE_FLAG(dest, flag) (dest & flag) ? dest &= ~flag : dest |= flag

// Used to check if a flag is toggled of or not
#define CHECK_FLAG(dest, flag) dest & flag

struct G
{
	ID3D12Device*               device              = nullptr;
    ID3D12CommandAllocator*     commandAllocator    = nullptr;
    ID3D12CommandQueue*         commandQueue        = nullptr;
    ID3D12GraphicsCommandList*  commandList         = nullptr;
};

extern G g;