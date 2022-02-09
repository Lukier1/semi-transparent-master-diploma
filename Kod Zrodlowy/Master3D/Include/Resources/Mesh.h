#pragma once

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Resource.h"

namespace Resources {
	class Mesh : public Resource {
	public:
		void LoadMesh(const aiMesh * mesh, bool smoothNormals);

		void GenBuffers();
		void DeleteBuffers();

		unsigned int getMaterialIndex() const;
		Mesh();
		Mesh(const aiMesh * mesh, bool smoothVertices);
		~Mesh();
		int GetVao();
		void Draw();
	
		// Inherited via Resource
		virtual void Release() override;
		virtual std::string GetFileName() const override;
	
	private:
		Logger * LOGGER = Logger::getLogger();

		unsigned int verticesCount = 0, trianglesCount = 0;

		GLfloat * points = NULL;
		GLfloat * normals = NULL;
		GLfloat * texcoords = NULL;
		GLfloat * tangents = NULL;

		GLuint vao = 0;

		unsigned int materialIndex = 0;;
		void LoadScene(const aiScene * scene);

		std::string mFilename;
	};
}

