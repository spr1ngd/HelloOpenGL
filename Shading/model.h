#pragma once
#include "GPUProgram.h"

struct VertexData
{
	float position[3];
	float normal[3];
	float texcoord[2];
};

VertexData* LoadObjModel(const char* filePath, unsigned int** indices, int& indexCount, int& vertexCount);

void DrawModel( const char* filePath, GPUProgram program,void* func()) ;