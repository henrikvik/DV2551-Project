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

constexpr char* hr_to_str(HRESULT hr)
{
	switch (hr) 
	{
	case 0x00000000: return "S_OK, Operation successful";
	case 0x80004001: return "E_NOTIMPL, Not implemented";
	case 0x80004002: return "E_NOINTERFACE, No such interface supported";
	case 0x80004003: return "E_POINTER, Pointer that is not valid";
	case 0x80004004: return "E_ABORT, Operation aborted";
	case 0x80004005: return "E_FAIL, Unspecified failure";
	case 0x8000FFFF: return "E_UNEXPECTED, Unexpected failure";
	case 0x80070005: return "E_ACCESSDENIED, General access denied error";
	case 0x80070006: return "E_HANDLE, Handle that is not valid";
	case 0x8007000E: return "E_OUTOFMEMORY, Failed to allocate necessary memory";
	case 0x80070057: return "E_INVALIDARG, One or more arguments are not valid";
	case 0x887A002B: return "DXGI_ERROR_ACCESS_DENIED, You tried to use a resource to which you did not have the required access privileges. This error is most typically caused when you write to a shared resource with read-only access.";
	case 0x887A0026: return "DXGI_ERROR_ACCESS_LOST, The desktop duplication interface is invalid. The desktop duplication interface typically becomes invalid when a different type of image is displayed on the desktop.";
	case 0x887A0036: return "DXGI_ERROR_ALREADY_EXISTS, The desired element already exists. This is returned by DXGIDeclareAdapterRemovalSupport if it is not the first time that the function is called.";
	case 0x887A002A: return "DXGI_ERROR_CANNOT_PROTECT_CONTENT, DXGI can't provide content protection on the swap chain. This error is typically caused by an older driver, or when you use a swap chain that is incompatible with content protection.";
	case 0x887A0006: return "DXGI_ERROR_DEVICE_HUNG, The application's device failed due to badly formed commands sent by the application. This is an design-time issue that should be investigated and fixed.";
	case 0x887A0005: return "DXGI_ERROR_DEVICE_REMOVED, The video card has been physically removed from the system, or a driver upgrade for the video card has occurred. The application should destroy and recreate the device. For help debugging the problem, call ID3D10Device::GetDeviceRemovedReason.";
	case 0x887A0007: return "DXGI_ERROR_DEVICE_RESET, The device failed due to a badly formed command. This is a run-time issue; The application should destroy and recreate the device.";
	case 0x887A0020: return "DXGI_ERROR_DRIVER_INTERNAL_ERROR, The driver encountered a problem and was put into the device removed state.";
	case 0x887A000B: return "DXGI_ERROR_FRAME_STATISTICS_DISJOINT, An event (for example, a power cycle) interrupted the gathering of presentation statistics.";
	case 0x887A000C: return "DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE, The application attempted to acquire exclusive ownership of an output, but failed because some other application (or device within the application) already acquired ownership.";
	case 0x887A0001: return "DXGI_ERROR_INVALID_CALL, The application provided invalid parameter data; this must be debugged and fixed before the application is released.";
	case 0x887A0003: return "DXGI_ERROR_MORE_DATA, The buffer supplied by the application is not big enough to hold the requested data.";
	case 0x887A002C: return "DXGI_ERROR_NAME_ALREADY_EXISTS, The supplied name of a resource in a call to IDXGIResource1::CreateSharedHandle is already associated with some other resource.";
	case 0x887A0021: return "DXGI_ERROR_NONEXCLUSIVE, A global counter resource is in use, and the Direct3D device can't currently use the counter resource.";
	case 0x887A0022: return "DXGI_ERROR_NOT_CURRENTLY_AVAILABLE, The resource or request is not currently available, but it might become available later.";
	case 0x887A0002: return "DXGI_ERROR_NOT_FOUND, When calling IDXGIObject::GetPrivateData, the GUID passed in is not recognized as one previously passed to IDXGIObject::SetPrivateData or IDXGIObject::SetPrivateDataInterface. When calling IDXGIFactory::EnumAdapters or IDXGIAdapter::EnumOutputs, the enumerated ordinal is out of range.";
	case 0x887A0023: return "DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED, Reserved";
	case 0x887A0024: return "DXGI_ERROR_REMOTE_OUTOFMEMORY, Reserved";
	case 0x887A0029: return "DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE, The DXGI output (monitor) to which the swap chain content was restricted is now disconnected or changed.";
	case 0x887A002D: return "DXGI_ERROR_SDK_COMPONENT_MISSING, The operation depends on an SDK component that is missing or mismatched.";
	case 0x887A0028: return "DXGI_ERROR_SESSION_DISCONNECTED, The Remote Desktop Services session is currently disconnected.";
	case 0x887A0004: return "DXGI_ERROR_UNSUPPORTED, The requested functionality is not supported by the device or the driver.";
	case 0x887A0027: return "DXGI_ERROR_WAIT_TIMEOUT, The time-out interval elapsed before the next desktop frame was available.";
	case 0x887A000A: return "DXGI_ERROR_WAS_STILL_DRAWING, The GPU was busy at the moment when a call was made to perform an operation, and did not execute or schedule the operation.";
	}

	return "UNKNOW HRESULT";
}

#define BreakOnFail(hr)  {\
    if FAILED(hr) \
    { \
        OutputDebugString(_bstr_t("\n---==+==---\n"));\
        OutputDebugString(_bstr_t(hr_to_str(hr)));\
        OutputDebugString(_bstr_t("\n"));\
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
        OutputDebugString(_bstr_t(hr_to_str(hr)));\
        OutputDebugString(_bstr_t("\n"));\
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
        OutputDebugString(_bstr_t(hr_to_str(hr)));\
        OutputDebugString(_bstr_t("\n"));\
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

	// CB Desc Heap
	ID3D12DescriptorHeap*		cbdHeap = nullptr;

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