#include "stdafx.h"
#include <Resources\Mesh.h>
#include <Math\Vec.h>

using namespace Resources;

void Mesh::LoadScene(const aiScene * scene)
{
	if (scene->HasMeshes())
	{
		LOGGER->Info("Loaded file contains " + std::to_string(scene->mNumMeshes) + " meshes");
		const aiMesh * mesh = scene->mMeshes[0];
		LoadMesh(mesh, false);

	}
	else
	{
		LOGGER->Error("Can't load mesh from scene");
	}
}
void Mesh::LoadMesh(const aiMesh * mesh, bool smoothNormals) {
	LOGGER->Info("Loaded meshes with " + std::to_string(mesh->mNumVertices) + " vertices");
	verticesCount = mesh->mNumVertices;
	trianglesCount = mesh->mNumFaces;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	int memInd = 0;
	points = new GLfloat[3 * 3 * mesh->mNumFaces];
	normals = new GLfloat[3 * 3 * mesh->mNumFaces];
	texcoords = new GLfloat[2 * 3 * mesh->mNumFaces];
	tangents =  new GLfloat[4 * 3 * mesh->mNumFaces];
	verticesCount = 3 * mesh->mNumFaces;

	materialIndex = mesh->mMaterialIndex;

	std::cout << "Material index: " << materialIndex << std::endl;
	for (unsigned int x = 0; x < mesh->mNumFaces; ++x)
	{
		const aiFace face = mesh->mFaces[x];
		for (unsigned int z = 0; z < face.mNumIndices; ++z)
		{
			int vertInd = face.mIndices[z];
			if (mesh->HasPositions())
			{
				const aiVector3D* vp = &(mesh->mVertices[vertInd]);
				points[memInd * 3] = (GLfloat)vp->x;
				points[memInd * 3 + 1] = (GLfloat)vp->y;
				points[memInd * 3 + 2] = (GLfloat)vp->z;
			}
			if (mesh->HasNormals())
			{

				const aiVector3D* vn = &(mesh->mNormals[vertInd]);
				normals[memInd * 3] = (GLfloat)vn->x;
				normals[memInd * 3 + 1] = (GLfloat)vn->y;
				normals[memInd * 3 + 2] = (GLfloat)vn->z;
			}
			if (mesh->HasTextureCoords(0))
			{

				const aiVector3D* vt = &(mesh->mTextureCoords[0][vertInd]);
				texcoords[memInd * 2] = (GLfloat)vt->x;
				texcoords[memInd * 2 + 1] = (GLfloat)vt->y;

			}
			if (mesh->HasTangentsAndBitangents())
			{

				const aiVector3D* vt = &(mesh->mTangents[vertInd]);
				const aiVector3D* vn = &(mesh->mNormals[vertInd]);
				const aiVector3D* vbt = &(mesh->mBitangents[vertInd]);
	
				Vec3 tang = Vector3f(vt->x, vt->y, vt->z);
				Vec3 norm = Vector3f(vn->x, vn->y, vn->z);
				Vec3 bitang = Vector3f(vbt->x, vbt->y, vbt->z);

				Vec3 tang_i = ( tang - (norm * norm.dot(tang)) ).normalizeVec3();
				float det = (tang.cross(norm)).dot(bitang);

				if (det < 0.0f)
				{
					det = -1.0f;
				}
				else
				{
					det = 1.0f;
				}
				tangents[memInd * 4] = (GLfloat)tang_i.v[0];
				tangents[memInd * 4 + 1] = (GLfloat)tang_i.v[1];
				tangents[memInd * 4 + 2] = (GLfloat)tang_i.v[2];
				tangents[memInd * 4 + 3] = (GLfloat)det;
			}
			++memInd;
		}
	}

	
	if (smoothNormals) {
		bool * activeVert = new bool[memInd];
		for (int i = 0; i < memInd; ++i) {
			activeVert[i] = true;
		}
		for (int i = 0; i < memInd; ++i) {
			if (!activeVert[i])
				continue;
			
			std::vector<int> selectedVertices;
			Vec3 first = Vector3f(points[3 * i], points[3 * i + 1], points[3 * i + 2]);
			Vec3 normalSum = Vector3f(normals[3 * i], normals[3 * i + 1], normals[3 * i + 2]);
			selectedVertices.push_back(i);
			for (int x = i + 1; x < memInd; ++x) {
				if (!activeVert[x])
					continue;
				Vec3 cmpPoint = Vector3f(points[3 * x], points[3 * x + 1], points[3 * x + 2]);
				if (cmpPoint == first) {
					activeVert[x] = false;
					normalSum = normalSum + Vector3f(normals[3 * x], normals[3 * x + 1], normals[3 * x + 2]);
					selectedVertices.push_back(x);
				}
			}

			normalSum = normalSum.normalizeVec3();

			for (int selected : selectedVertices)
			{
				normals[3 * selected] = normalSum.v[0];
				normals[3 * selected + 1] = normalSum.v[1];
				normals[3 * selected + 2] = normalSum.v[2];;
			}
		}
		delete[] activeVert;
	}


	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * verticesCount * sizeof(float), points, GL_STATIC_DRAW);
	GLuint normals_vbo = 0;
	glGenBuffers(1, &normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * verticesCount  * sizeof(float), normals, GL_STATIC_DRAW);
	GLuint tc_vbo = 0;
	glGenBuffers(1, &tc_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, tc_vbo);
	glBufferData(GL_ARRAY_BUFFER, 2 * verticesCount  * sizeof(float), texcoords, GL_STATIC_DRAW);
	GLuint tan_vbo = 0;
	glGenBuffers(1, &tan_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, tan_vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * verticesCount * sizeof(float), tangents, GL_STATIC_DRAW);


	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindVertexBuffer(0, vbo, 0, 3 * sizeof(GL_FLOAT));
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);
	glEnableVertexAttribArray(1);
	glBindVertexBuffer(1, normals_vbo, 0, 3 * sizeof(GL_FLOAT));
	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(1, 1);
	glEnableVertexAttribArray(2);
	glBindVertexBuffer(2, tc_vbo, 0, 2 * sizeof(GL_FLOAT));
	glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(2, 2);
	glEnableVertexAttribArray(3);
	glBindVertexBuffer(3, tan_vbo, 0, 4 * sizeof(GL_FLOAT));
	glVertexAttribFormat(3, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(3, 3);

}
void Mesh::GenBuffers()
{ }
void Mesh::DeleteBuffers()
{

}
unsigned int Mesh::getMaterialIndex() const
{
	return materialIndex;
}
Mesh::Mesh()
{
}
Mesh::Mesh(const aiMesh * mesh, bool smothVertices)
{
	LoadMesh(mesh, smothVertices);
}


Mesh::~Mesh() {
		Release();
	}
int Mesh::GetVao() {
		return vao;
	}
void Mesh::Draw()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, verticesCount);
}
void Mesh::Release() {
	delete[] points;
	points = nullptr;
	delete[] normals;
	normals = nullptr;
	delete[] texcoords;
	texcoords = nullptr;
	delete[] tangents;
	tangents = nullptr;
}

std::string Mesh::GetFileName() const
{
	return mFilename;
}



