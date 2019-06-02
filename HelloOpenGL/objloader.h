#pragma once
#include "vertexData.h"

class ObjLoader
{
public:
	vertexData*mVertexes;
	int *mIndices;
	int mIndexCount;
	void Init(const char* objModel);
	void Draw();
};

