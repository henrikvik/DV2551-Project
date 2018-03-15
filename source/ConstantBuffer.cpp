#include "ConstantBuffer.h" 

ConstantBuffer::ConstantBuffer(UINT offset, UINT max, UINT width, ID3D12DescriptorHeap *descHeap) {
	this->max = max;
	this->offset = offset;
	this->width = width;
	resource = nullptr;

	createConstantBuffer();
	createView(descHeap);
	uploadDataToResource();
}

ConstantBuffer::~ConstantBuffer()
{
	SafeRelease(resource);
}

void ConstantBuffer::createConstantBuffer() {
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
	resDesc.DepthOrArraySize = resDesc.Height = resDesc.MipLevels = 1;
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resDesc.Width = sizeof(float) * width;

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	BreakOnFail(g.device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource)
	));
}

void ConstantBuffer::createView(ID3D12DescriptorHeap *descHeap)
{
	static UINT size = g.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    auto cdh = descHeap->GetCPUDescriptorHandleForHeapStart();
    cdh.ptr += g.cbdHeapLen++ * size;

	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.BufferLocation = resource->GetGPUVirtualAddress();
	desc.SizeInBytes = ((sizeof(float) * width) + 255) & ~255; // magic
	g.device->CreateConstantBufferView(&desc, cdh);
}

void ConstantBuffer::uploadDataToResource()
{
	assert(resource != nullptr);

	float *data = new float[width];
	for (int i = 0; i < width; i++)
		data[i] = (rand() % max) + offset;

	D3D12_RANGE range{ 0, 0 };
	void *memory;

	resource->Map(0, &range, &memory);
	memcpy(memory, data, width);
	resource->Unmap(0, nullptr);
}

UINT ConstantBuffer::getMax() const {
	return max;
}

UINT ConstantBuffer::getWidth() const {
	return width;
}

UINT ConstantBuffer::getOffset() const
{
	return offset;
}

ID3D12Resource * ConstantBuffer::getResource() const
{
	return resource;
}

void ConstantBuffer::CreateDescHeap(ID3D12DescriptorHeap*& descHeap, UINT numDescriptors)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = numDescriptors;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	BreakOnFail(g.device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descHeap)));
}
