#include "ConstantBuffer.h" 

ConstantBuffer::ConstantBuffer(UINT min, UINT max) {
	this->min = min;
	this->max = max;
	createConstantBuffer();
}

void createConstantBuffer() {
	
}