#pragma once
#include "fbxsdk.h"
#pragma comment(lib,"libfbxsdk-md.lib")
#include "vector3f.h"
#include <vector>
#include "vertexData.h"
#include <unordered_map>
#include "texture.h"

struct Mat 
{
	int mType;
	char mDiffuseColorTexture[256];
	Texture* texture;
};

class FBXModel
{
protected:
	void ImportNode(FbxNode* node);
	void ImportMaterial(FbxNode* node);
	void ImportMesh(FbxMesh* mesh);

public:
	char fbxPath[256];
	std::vector<Mat*> materials;
	std::unordered_map<int, int> materialIndex;
	std::vector<std::vector<int>> indices;
	vertexData* vertices;
	void Init(const char*filePath);
	void Draw();
};

