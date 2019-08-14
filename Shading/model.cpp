
#include "util.h"
#include "model.h"
#include "misc.h"
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>

VertexData* LoadObjModel(const char* filePath, unsigned int** indices, int& indexCount, int& vertexCount)
{
	char* fileContent = LoadFileContent(filePath);
	if (fileContent != nullptr)
	{
		struct VertexInfo 
		{
			float v[3];
		};
		struct VertexIndex 
		{
			int vertexIndex;
			int normalIndex;
			int texcoordIndex;
		};
		std::vector<VertexInfo> positions;
		std::vector<VertexInfo> normals;
		std::vector<VertexInfo> texcoords;
		std::vector<VertexIndex> vertices;
		std::vector<int> mIndices;
			
		std::stringstream ssObjectFile(fileContent);
		std::string temp;
		char szOneLine[256];
		while (!ssObjectFile.eof()) 
		{
			memset(szOneLine, 0, 256);
			ssObjectFile.getline(szOneLine, 256);
			if (strlen(szOneLine) > 0)
			{
				std::stringstream ssOneLine(szOneLine);
				if (szOneLine[0] == 'v') 
				{ 
					if (szOneLine[1] == 't') 
					{
						// texcoord
						//printf("texcoord : %s\n",szOneLine);
						ssOneLine >> temp;
						VertexInfo texcoord;
						ssOneLine >> texcoord.v[0];
						ssOneLine >> texcoord.v[1];
						texcoords.push_back(texcoord);
					}
					else if (szOneLine[1] == 'n') 
					{
						// normal
						//printf("normal : %s\n", szOneLine);
						ssOneLine >> temp;
						VertexInfo normal;
						ssOneLine >> normal.v[0];
						ssOneLine >> normal.v[1];
						ssOneLine >> normal.v[2];
						normals.push_back(normal);
					}
					else
					{
						// vertex
						//printf("vertex : %s\n", szOneLine);
						ssOneLine >> temp;
						VertexInfo vertex;
						ssOneLine >> vertex.v[0];
						ssOneLine >> vertex.v[1];
						ssOneLine >> vertex.v[2];
						positions.push_back(vertex);
					}
				}
				else if (szOneLine[0] == 'f')
				{
					//printf("face : %s\n", szOneLine);
					ssOneLine >> temp;
					std::string indexStr;
					for (int i = 0; i < 3; i++) 
					{
						ssOneLine >> indexStr;
						size_t pos1 = indexStr.find_first_of('/');
						std::string vertexIndex =  indexStr.substr(0, pos1);
						size_t pos2 = indexStr.find_first_of('/', pos1 + 1);
						std::string texcoordIndex = indexStr.substr(pos1 +1 , pos2 - (pos1 + 1));
						std::string noramlIndex = indexStr.substr(pos2 + 1 ,indexStr.length() - (pos2 + 1));
						VertexIndex index;
						index.vertexIndex = atoi(vertexIndex.c_str());
						index.normalIndex = atoi(noramlIndex.c_str());
						index.texcoordIndex = atoi(texcoordIndex.c_str());

						// »•÷ÿ∏¥
						int currentIndex = -1;
						int indexCount = (int)vertices.size();
						for (int i = 0; i < indexCount; i++) 
						{
							if (vertices[i].vertexIndex == index.vertexIndex &&
								vertices[i].normalIndex == index.normalIndex &&
								vertices[i].texcoordIndex == index.texcoordIndex) 
							{
								currentIndex = i;
								break;
							}
						}
						if (currentIndex == -1)
						{
							currentIndex = (int)vertices.size();
							vertices.push_back(index);
						}
						mIndices.push_back(currentIndex);
					}
				}
			}
		}
		indexCount = (int)mIndices.size();
		vertexCount = (int)vertices.size();
		*indices = new unsigned int[indexCount];
		for (int i = 0; i < indexCount; i++)
			(*indices)[i] = mIndices[i];
		VertexData* vertexDatas = new VertexData[vertexCount];
		for (int i = 0; i < vertexCount; i++)
		{
			int vertexIndex = vertices[i].vertexIndex;
			int normalIndex = vertices[i].normalIndex;
			int texcoordIndex = vertices[i].texcoordIndex;
			memcpy(vertexDatas[i].position, positions[vertexIndex-1].v, sizeof(float) * 3);
			memcpy(vertexDatas[i].normal,normals[normalIndex - 1].v,sizeof(float)*3);
			memcpy(vertexDatas[i].texcoord,texcoords[texcoordIndex - 1].v,sizeof(float)*2);
		}
		return vertexDatas;
	}
	return nullptr;
}

void DrawModel( const char* filePath , GPUProgram program ,GLuint&vao ,GLuint&ibo,int& indexCount)   
{
	unsigned int* indices = nullptr;
	int indicesCount, vertexCount;
	VertexData* vertices = LoadObjModel(filePath, &indices, indicesCount, vertexCount);
	//indexCount = indicesCount;

	// create vao
	/*GLuint*/ vao = CreateVAO([&](void)
	{
		GLuint vbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * vertexCount, GL_STATIC_DRAW, vertices);
		// bind vbo
		
		glBindBuffer(GL_ARRAY_BUFFER,vbo);

		glEnableVertexAttribArray(program.GetLocation(VERTEX));
		glVertexAttribPointer(program.GetLocation(VERTEX), sizeof(float) * 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);

		glEnableVertexAttribArray(program.GetLocation(NORMAL));
		glVertexAttribPointer(program.GetLocation(NORMAL),sizeof(float)*3,GL_FLOAT,GL_FALSE,sizeof(VertexData),(void*)(sizeof(float)*3));

		glEnableVertexAttribArray(program.GetLocation(TEXCOORD));
		glVertexAttribPointer(program.GetLocation(TEXCOORD), sizeof(float) * 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 5));

		glBindBuffer(GL_ARRAY_BUFFER,0);
	});

	// create ibo
	/*GLuint*/ ibo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* indicesCount,GL_STATIC_DRAW,indices);

	/*glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, (void*)0);
	void*func();
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);*/
};