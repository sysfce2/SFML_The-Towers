#include "OutlineRenderer.hpp"

void OutlineRenderer::init(){

     glGenVertexArrays(1, &m_vaoID);
     glBindVertexArray(m_vaoID);

     glGenBuffers(1, &m_vboID);
     glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

     glEnableVertexAttribArray(0);
     glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(double) * 3, 0);

     double vertices[] = {
          -0.001, -0.001, -0.001,
          -0.001, 1.001, -0.001,
          1.001, 1.001, -0.001,
          1.001, -0.001, -0.001,
          -0.001, -0.001, 1.001,
          -0.001, 1.001, 1.001,
          1.001, 1.001, 1.001,
          1.001, -0.001, 1.001,
     };

     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
     glBindBuffer(GL_ARRAY_BUFFER, 0);

     glGenBuffers(1, &m_eboID);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboID);

     unsigned int indices[] = {
          0, 1, 2, 0, 2, 3,
          1, 5, 6, 1, 6, 2,
          4, 6, 5, 4, 7, 6,
          4, 5, 1, 4, 1, 0,
          3, 2, 6, 3, 6, 7,
          4, 0, 3, 4, 3, 7
     };

     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


     glBindVertexArray(0);

}

void OutlineRenderer::renderFace(Face face){

     glBindVertexArray(m_vaoID);

     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboID);

     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)((uint64_t)face * 6 * 4));

     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

     glBindVertexArray(0);

}

void OutlineRenderer::destroy(){
     glDeleteBuffers(1, &m_vboID);
	glDeleteBuffers(1, &m_eboID);
     glDeleteVertexArrays(1, &m_vaoID);
}
