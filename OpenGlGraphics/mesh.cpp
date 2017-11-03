#include "mesh.h"
#include <vector>


Mesh::Mesh(Vertex* vertices, unsigned int numVertices)
{
	//m_display = display;

	this->vertices.reserve(numVertices);
	for (int i = 0; i < numVertices; i++)
		this->vertices.push_back(vertices[i]);

	//m_indecies.reserve(numIndecies);
	//for (int i = 0; i < numIndecies; i++)
	//	m_indecies.push_back(indecies[i]);


	m_initialized = true;
	m_drawCount = numVertices;
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;

	positions.reserve(numVertices);
	texCoords.reserve(numVertices);

	for (unsigned i = 0; i < numVertices; i++)
	{
		positions.push_back(vertices[i].pos);
		texCoords.push_back(vertices[i].texCoord);
	}

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//glBindVertexArray(0);

	//glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(texCoords[0]), &texCoords[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndecies * sizeof(m_indecies[0]), &m_indecies[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	//if(m_initialized)
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh::Draw()
{
	//m_display->AppendToDrawBuffer(&(vertices[0]), 36, &glm::vec3(0,0,0));

	//glBindVertexArray(m_vertexArrayObject);

	///glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, drawCount);

	//glBindVertexArray(0);
}
