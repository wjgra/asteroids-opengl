#include "../include/drawable.hpp"

Drawable::Drawable(){
    // Generate buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

Drawable::~Drawable(){
    // Delete buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Drawable::setUpBuffers(){
    // Bind VAO
    glBindVertexArray(VAO); 

    // Bind VBO and copy vertex data into VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices.data()), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Bind EBO and copy element data into EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements.data()), elements.data(), GL_STATIC_DRAW);
}

void Drawable::setVertices(std::vector<float> verts){
    vertices = verts;
}

float const *Drawable::getVertices() const{
    return vertices.data();
}

void Drawable::setElements(std::vector<GLuint> elts){
    elements = elts;
}

GLuint const *Drawable::getElements() const{
    return elements.data();
}