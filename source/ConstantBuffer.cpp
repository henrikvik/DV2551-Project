#include "ConstantBuffer.h" 

ConstantBuffer::ConstantBuffer(UINT max, UINT width) {
	this->max = max;
	this->width = width;
	resource = nullptr;

	createConstantBuffer();
	uploadDataToResource();
}

ConstantBuffer::~ConstantBuffer()
{
	SafeRelease(resource);
}

void ConstantBuffer::createConstantBuffer() {
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
	resDesc.DepthOrArraySize, resDesc.Height, resDesc.MipLevels = 1;
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resDesc.Width = width;

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

void ConstantBuffer::uploadDataToResource()
{
	assert(resource != nullptr);

	float *data = new float[width];
	for (int i = 0; i < width; i++)
		data[i] = rand() % max;

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

ID3D12Resource * ConstantBuffer::getResource() const
{
	return resource;
}
