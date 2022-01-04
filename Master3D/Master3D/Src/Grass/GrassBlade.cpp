
#include "stdafx.h"

#include <Grass\GrassBlade.h>


GLfloat POINTS_POS[] = {
	-0.1f, -1.0f, 0.0f,
	0.1f, -1.0f, 0.0f,
	-0.1f, 1.0f, 0.0f,

	-0.1f, 1.0f, 0.0f,
	0.1f, -1.0f, 0.0f,
	0.1f, 1.0f, 0.0f,

	-0.1f, 1.0f, 0.0f,
	0.1f, 1.0f, 0.0f,
	-0.1f, 3.0f, 0.0f,
	
	-0.1f, 3.0f, 0.0f,
	0.1f, 1.0f, 0.0f,
	0.1f, 3.0f, 0.0f
};

const GLfloat POINT_TEXCOORD[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 0.5f,

	0.0f, 0.5f, 
	1.0f, 0.0f,
	1.0f, 0.5f,

	0.0f, 0.5f,
	1.0f, 0.5f,
	0.0f, 0.99f,

	0.0f, 0.99f,
	1.0f, 0.5f,
	1.0f, 0.99f,
};

const GLfloat POINT_NORMALS[] = {
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f
};
GrassBlade::GrassBlade() : mTexture("Data/Images/grass.png", Resources::TextureType::DIFFUSE_TEXTURE) {
	for (int i = 0; i < 36; ++i) {
		POINTS_POS[i] = POINTS_POS[i] * 0.1f;
	}


	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(POINTS_POS), &POINTS_POS, GL_STATIC_DRAW);
	GLuint normals_vbo = 0;
	glGenBuffers(1, &normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(POINT_NORMALS), &POINT_NORMALS, GL_STATIC_DRAW);
	GLuint tc_vbo = 0;
	glGenBuffers(1, &tc_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, tc_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(POINT_TEXCOORD), &POINT_TEXCOORD, GL_STATIC_DRAW);

	mVAO = 0;
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
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
}

void  GrassBlade::Render(const Matrix4x4 & V, const Matrix4x4 &P, const Resources::ShaderProgramPtr shader) {
	shader->applyTexture(mTexture.getTextureType(), mTexture.getTexID());

	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(POINTS_POS)/sizeof(GLfloat)/3);
}