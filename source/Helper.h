#pragma once
#include <d3d12.h>
#

#define SafeRelease(ptr) {if(ptr){ptr->Release();ptr=nullptr;}}

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


struct G
{
	ID3D12Device * device;
};

extern G g;