#include "FBXModel.h"
#include <windows.h>
#include <gl/GL.h>

void ImportPositions(FbxGeometryBase* geometry, std::vector<Vector3f>& positions)
{
	int controlPointCount = geometry->GetControlPointsCount();
	positions.resize(controlPointCount);

	printf("contron point count : %d\n", controlPointCount);

	FbxVector4* controlPoints = geometry->GetControlPoints();
	for (int i = 0; i < controlPointCount; i++)
	{
		positions[i].x = static_cast<float>(controlPoints[i].mData[0]);
		positions[i].y = static_cast<float>(controlPoints[i].mData[1]);
		positions[i].z = static_cast<float>(controlPoints[i].mData[2]);
		//printf("vertex position : %f,%f,%f\n", positions[i].x, positions[i].y, positions[i].z);
	}
}

void GetTexturePath(char* path ) 
{
	// 把fbx path转化为 .fbm/
	memset(path + strlen(path) - 4, 0, 4);
	strcat(path, ".fbm/");
}

void FBXModel::Init(const char* filePath) 
{
	memset(fbxPath, 0, 256);
	strcpy(fbxPath, filePath);
	printf("fbx path : %s \n", fbxPath);

	FbxManager* manager = FbxManager::Create();
	FbxIOSettings* setting = FbxIOSettings::Create(manager,IOSROOT);
	FbxImporter* fbxImprot = FbxImporter::Create(manager,"");
	manager->SetIOSettings(setting);

	if (fbxImprot->Initialize(filePath, -1, manager->GetIOSettings())) 
	{
		FbxScene* scene = FbxScene::Create(manager, "fbxmodel");
		fbxImprot->Import(scene);

		FbxNode* rootNode = scene->GetRootNode();
		ImportNode(rootNode);
		scene->Destroy();
	}

	fbxImprot->Destroy();
	setting->Destroy();
	manager->Destroy();
}

void FBXModel::ImportNode(FbxNode*node) 
{
	//printf("decode node : %s \n",node->GetName());

	if (node->GetMesh()) 
	{
		printf("decode mesh : %s \n",node->GetName());
		ImportMaterial(node);
		ImportMesh(node->GetMesh());		
		for (auto iter = materials.begin(); iter != materials.end(); iter++)
		{
			printf("%d %s \n",(*iter)->mType,(*iter)->mDiffuseColorTexture);
		}
		for (auto iter = materialIndex.begin(); iter != materialIndex.end(); iter++) 
		{
			printf("%d -> %d \n",iter->first,iter->second);
		}
	}

	int childNode = node->GetChildCount();
	for (int i = 0; i < childNode; i++)
	{
		ImportNode(node->GetChild(i));
	}
}

void FBXModel::ImportMaterial(FbxNode* node) 
{
	char fbxTexturePath[256] = { 0 };
	strcpy(fbxTexturePath,fbxPath);
	GetTexturePath(fbxTexturePath);
	int matCount = node->GetMaterialCount(); 
	FbxProperty property;
	int nType = 0;
	for (int i = 0; i < matCount; i++) 
	{
		FbxSurfaceMaterial* surface = node->GetMaterial(i);
		if (FbxSurfaceLambert::ClassId == surface->GetClassId()) 
		{
			nType = 0; 
		}
		else if ( FbxSurfacePhong::ClassId == surface->GetClassId() ) 
		{
			nType = 1; 
		}
		else 
		{
			printf(" can't load this matrial.");
		}
		property = surface->FindProperty(FbxLayerElement::sTextureChannelNames[0]); // diffuse texture
		if (property.IsValid())
		{
			int textureCount = property.GetSrcObjectCount<FbxTexture>();
			/*for (int j = 0; j < textureCount; j++) 
			{

			}*/
			int currentMatIndex = -1;
			FbxTexture* texture = property.GetSrcObject<FbxTexture>();
			if (texture) 
			{ 
				int currentMatCount = materials.size();
				for (int k = 0; k < currentMatCount; k++)
				{
					if (nType == materials[k]->mType )
					{
						if (strcmp(texture->GetName(), materials[k]->mDiffuseColorTexture) == 0) 
						{
							currentMatIndex = k;
							break;
						}
					}
				}
				if (currentMatIndex == -1) 
				{
					Mat* mat = new Mat;
					mat->mType = nType;
					memset(mat->mDiffuseColorTexture,0,256);
					strcpy(mat->mDiffuseColorTexture,texture->GetName());

					char texturePath[256] = { 0 };
					strcpy(texturePath,fbxTexturePath);
					strcat(texturePath, mat->mDiffuseColorTexture); 
					mat->texture = Texture::LoadTexture(texturePath); 
					printf("load texture: %s ----> textureID %d \n", texturePath,mat->texture->mTextureID);
					currentMatIndex = materials.size();
					materials.push_back(mat);
				}
				materialIndex.insert(std::pair<int,int>(i, currentMatIndex));
			}
		}
	} 
}

void FBXModel::ImportMesh(FbxMesh* mesh) 
{
	std::vector<Vector3f> positions;
	ImportPositions(mesh, positions);

	// indices triangle index.
	int polygonCount = mesh->GetPolygonCount();

	int pointCount = 0;
	for (int i = 0; i < polygonCount; i++)
	{
		pointCount+= mesh->GetPolygonSize(i);
	}
	FbxLayerElementArrayTemplate<int>* pMaterialIndices;
	mesh->GetMaterialIndices(&pMaterialIndices);
	FbxGeometryElement::EMappingMode mode = mesh->GetElementMaterial()->GetMappingMode();
	indices.resize(materials.size());
	printf("indices size %d \n",indices.size());

	vertices = new vertexData[pointCount];

	FbxStringList uvSetNames;
	mesh->GetUVSetNames(uvSetNames); // TEXCOORD0 TEXCOORD1	

	int verticesIndex = 0; 
	 
	for (int i = 0; i < polygonCount; i++) 
	{
		int currentPolygonIndexCount = mesh->GetPolygonSize(i); 
		for (int j = 0; j < currentPolygonIndexCount; j++) 
		{ 
			int vertexIndex = mesh->GetPolygonVertex(i,j);
			vertices[verticesIndex].position[0] = static_cast<float>(positions[vertexIndex].x);
			vertices[verticesIndex].position[1] = static_cast<float>(positions[vertexIndex].y);
			vertices[verticesIndex].position[2] = static_cast<float>(positions[vertexIndex].z);

			FbxVector4 normal;
			if (mesh->GetPolygonVertexNormal(i, j, normal))
			{
				//printf("vertex normal : %f,%f,%f \n",normal.mData[0], normal.mData[1], normal.mData[2]);
				vertices[verticesIndex].normal[0] = static_cast<float>(normal.mData[0]);
				vertices[verticesIndex].normal[1] = static_cast<float>(normal.mData[1]);
				vertices[verticesIndex].normal[2] = static_cast<float>(normal.mData[2]);
			}

			FbxVector2 uv;
			bool bFlag = true;
			if (mesh->GetPolygonVertexUV(i, j, uvSetNames.GetItemAt(0)->mString.Buffer(), uv, bFlag))
			{
				//printf("vertex texcoord0 : %f,%f\n", uv.mData[0], uv.mData[1]);
				vertices[verticesIndex].texcoord[0] = static_cast<float>(uv.mData[0]);
				vertices[verticesIndex].texcoord[1] = static_cast<float>(uv.mData[1]);
			}

			verticesIndex++;
		}
	} 

	switch (mode)
	{
	case fbxsdk::FbxLayerElement::eNone:
		break;
	case fbxsdk::FbxLayerElement::eByControlPoint:
		break;
	case fbxsdk::FbxLayerElement::eByPolygonVertex:
		break;
	case fbxsdk::FbxLayerElement::eByPolygon:
	{
		for (int i = 0; i < polygonCount; i++)
		{
			int matIndex = pMaterialIndices->GetAt(i); // 当前三角面对应的材质序号
			int materialRef = materialIndex[matIndex]; // 获取真实的材质需要

			indices[materialRef].push_back(i * 3);
			indices[materialRef].push_back(i * 3 + 1);
			indices[materialRef].push_back(i * 3 + 2);
		}
		break;
	}
	case fbxsdk::FbxLayerElement::eByEdge:
		break;
	case fbxsdk::FbxLayerElement::eAllSame:
		break;
	default:
		break;
	}
} 

void FBXModel::Draw() 
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -5.0f);
	glScalef(0.01f,0.01f,0.01f);
	for (int i = 0; i < indices.size(); i++)
	{
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.01f);
		glBindTexture(GL_TEXTURE_2D, materials[i]->texture->mTextureID);
		std::vector<int> matVertices =  indices[i]; // 每个材质球对应的顶点 
		glBegin(GL_TRIANGLES); 
		for (int j = 0; j < matVertices.size(); j++)
		{ 
			glTexCoord2fv(vertices[matVertices[j]].texcoord);
			glVertex3fv(vertices[matVertices[j]].position);
		}
		glEnd();
	}
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();
}