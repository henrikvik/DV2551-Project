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
		/** NYI -- NYI -- Will boop */
		ConstantBuffer(UINT offset, UINT max, UINT width, ID3D12DescriptorHeap *descHeap);
		~ConstantBuffer();

		UINT getMax() const;
		UINT getWidth() const;
		UINT getOffset() const;
		ID3D12Resource* getResource() const;
	
		static void CreateDescHeap(ID3D12DescriptorHeap*& descHeap, UINT numDescriptors);
};