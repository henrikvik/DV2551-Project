#pragma once

#include "Helper.h"

class ConstantBuffer {
	private:
		ID3D12Resource *resource;
		UINT max, width;

		void createConstantBuffer();
		void uploadDataToResource();
	public:
		/** NYI -- NYI -- Will boop */
		ConstantBuffer(UINT max, UINT width);
		~ConstantBuffer();

		UINT getMax() const;
		UINT getWidth() const;
		ID3D12Resource* getResource() const;
};