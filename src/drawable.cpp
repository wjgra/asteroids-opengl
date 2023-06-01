#include "../include/drawable.hpp"

Drawable::Drawable(const std::vector<float>& verts, const std::vector<GLuint>& elts): vertices{verts}, elements{elts}
{
}

Drawable::~Drawable(){
}

void Drawable::setUpBuffers(){
    // Generate buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO); 

    // Bind VBO and copy vertex data into VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    

    // Bind EBO and copy element data into EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size()*sizeof(GLuint), elements.data(), GL_STATIC_DRAW);

    // Unbind VAO
    glBindVertexArray(0);
}

void Drawable::releaseBuffers(){
    // Delete buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

float const *Drawable::getVertices() const{
    return vertices.data();
}

GLuint const *Drawable::getElements() const{
    return elements.data();
}

void Drawable::draw(){
    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_LINE_STRIP, 5, GL_UNSIGNED_INT, 0, 9);
    glBindVertexArray(0);
}