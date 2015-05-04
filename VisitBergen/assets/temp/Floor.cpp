#include "Floor.h"


Floor::Floor()
{
}


Floor::~Floor()
{
}

void Floor::init()
{
	GLfloat vertices[] = {
		// position				// normal
		1.0f, 0.0f, -1.0f,		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, -1.0f,		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f
	};

	GLuint indices[] = {
		0, 1, 2,
		1, 2, 3
	};

	this->vertexBuffer = 0;
	glGenBuffers(1, &this->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	//glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), &vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	this->indexBuffer = 0;
	glGenBuffers(1, &this->indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(GLuint), &indices, GL_STATIC_DRAW);
}

void Floor::draw()
{
	//glUseProgram(this->shaderProgram->shaderProgram);
	//this->shaderProgram->updateModelUniform(this->modelMatrix);

	//GLint vertexLocation = this->shaderProgram->attribLocations["vertex"];
	//glEnableVertexAttribArray(vertexLocation);
	//glVertexAttribPointer(vertexLocation,
	//	3,
	//	GL_FLOAT,
	//	GL_FALSE,
	//	3 * sizeof(GLfloat),
	//	(void*)0);

	//GLint vertexNormalLocation = this->shaderProgram->attribLocations["normal"];
	//glEnableVertexAttribArray(vertexNormalLocation);
	//glVertexAttribPointer(vertexNormalLocation,
	//	3,
	//	GL_FLOAT,
	//	GL_FALSE,
	//	3 * sizeof(GLfloat),
	//	reinterpret_cast<const GLvoid*>(3 * sizeof(GLfloat)));


	//glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);

	//glDrawElements(GL_TRIANGLES, 2, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(0));

	////glDrawArrays(GL_TRIANGLES, 0, 2);


	//glDisableVertexAttribArray(vertexLocation);
	//glDisableVertexAttribArray(vertexNormalLocation);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	struct MyVertex
	{
		float x, y, z;        //Vertex
		float nx, ny, nz;     //Normal
		float s0, t0;         //Texcoord0
	};

	MyVertex pvertex[3];
	//VERTEX 0
	pvertex[0].x = 0.0;
	pvertex[0].y = 0.0;
	pvertex[0].z = 0.0;
	pvertex[0].nx = 0.0;
	pvertex[0].ny = 0.0;
	pvertex[0].nz = 1.0;
	pvertex[0].s0 = 0.0;
	pvertex[0].t0 = 0.0;
	//VERTEX 1
	pvertex[1].x = 1.0;
	pvertex[1].y = 0.0;
	pvertex[1].z = 0.0;
	pvertex[1].nx = 0.0;
	pvertex[1].ny = 0.0;
	pvertex[1].nz = 1.0;
	pvertex[1].s0 = 1.0;
	pvertex[1].t0 = 0.0;
	//VERTEX 2
	pvertex[2].x = 0.0;
	pvertex[2].y = 1.0;
	pvertex[2].z = 0.0;
	pvertex[2].nx = 0.0;
	pvertex[2].ny = 0.0;
	pvertex[2].nz = 1.0;
	pvertex[2].s0 = 0.0;
	pvertex[2].t0 = 1.0;

	GLuint VertexVBOID;
	glGenBuffers(1, &VertexVBOID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex) * 3, &pvertex[0].x, GL_STATIC_DRAW);

	USHORT pindices[3];
	pindices[0] = 0;
	pindices[1] = 1;
	pindices[2] = 2;

	GLuint IndexVBOID;
	glGenBuffers(1, &IndexVBOID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(USHORT) * 3, pindices, GL_STATIC_DRAW);


		glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
	glEnableVertexAttribArray(0);    //We like submitting vertices on stream 0 for no special reason
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), ((char *)NULL + (0)));   //The starting point of the VBO, for the vertices
	glEnableVertexAttribArray(1);    //We like submitting normals on stream 1 for no special reason
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), ((char *)NULL + (12)));     //The starting point of normals, 12 bytes away
	glEnableVertexAttribArray(2);    //We like submitting texcoords on stream 2 for no special reason
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertex), ((char *)NULL + (24)));   //The starting point of texcoords, 24 bytes away

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
	//To render, we can either use glDrawElements or glDrawRangeElements
	//The is the number of indices. 3 indices needed to make a single triangle
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, ((char *)NULL + (0)));   //The starting point of the IBO

}

void Floor::setShaderProgram(const std::shared_ptr<ShaderProgram> shaderProgram)
{
	this->shaderProgram = shaderProgram;
}