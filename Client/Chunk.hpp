#ifndef CHUNK_H
#define CHUNK_H
#include "glad.h"
#include <glm/glm.hpp>
#include <vector>
#include <cstddef>
#include <SFML/Graphics.hpp>
#include "Vertex.hpp"


class Chunk {
public:

	Chunk();
	~Chunk();
	void init(int x, int y, int z);

	void render();
	void destroy();
	void pushData(Vertex* vertices, unsigned int numVertices);

	void setX(int x);
	void setY(int y);
	void setZ(int z);

	int getX();
	int getY();
	int getZ();
	unsigned int getNumVertices();

	bool needsUpdate = true;

private:

	//Opengl Variables
	GLuint m_vaoID = 0;
	GLuint m_vboID = 0;
	GLuint m_numVertices = 0;
	int m_x = 0;
	int m_y = 0;
	int m_z = 0;

};

#endif
