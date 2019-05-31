#pragma once
#include "vertexData.h"

class ObjLoader
{
public:
	vertexData*mVertexes;
	int *mIndices;
	int mIndexCount;
	void init(const char* objModel);
	void Draw();
};

