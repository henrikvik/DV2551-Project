#pragma once

#include "Helper.h"

class ConstantBuffer {
	private:
		ID3D12Resource *resource;
		UINT max, width, offset;

		void createConstantBuffer();
		void createView(ID3D12DescriptorHeap *descHeap);
		void uploadDataToResource();
	public:
		ConstantBuffer(UINT offset = 0, UINT max = 100, UINT width = 1, ID3D12DescriptorHeap *descHeap = g.cbdHeap);
		~ConstantBuffer();

		UINT getMax() const;
		UINT getWidth() const;
		UINT getOffset() const;
		ID3D12Resource* getResource() const;
	
		static void CreateDescHeap(ID3D12DescriptorHeap*& descHeap, UINT numDescriptors);
};