#include "model.h"
#include "misc.h"
#include <stdio.h>
#include <string>
#include <sstream>

VertexData* LoadObjModel(const char* filePath, unsigned int** indices, int& indexCount, int& vertexCount)
{
	char* fileContent = LoadFileContent(filePath);
	if (fileContent != nullptr)
	{
		std::stringstream ssObjectFile(fileContent);
		char szOneLine[256];
		while (!ssObjectFile.eof()) 
		{
			memset(szOneLine, 0, 256);
			ssObjectFile.getline(szOneLine, 256);
			if (strlen(szOneLine) > 0)
			{
				if (szOneLine[0] == 'v') 
				{
					if (szOneLine[1] == 't') 
					{
						// texcoord
						printf("texcoord : %s\n",szOneLine);
					}
					else if (szOneLine[1] == 'n') 
					{
						// normal
						printf("normal : %s\n", szOneLine);
					}
					else
					{
						// vertex
						printf("vertex : %s\n", szOneLine);
					}
				}
				else if (szOneLine[0] == 'f')
				{
					printf("face : %s\n", szOneLine);
				}
			}
		}
	}
	return nullptr;
}