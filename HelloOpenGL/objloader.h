#pragma once
#include "vertexData.h"

class ObjLoader
{
public:
	vertexData*mVertexes;
	int* mIndices;
	void init(const char* objModel);
	void Draw();
};

