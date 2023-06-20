#include "../include/text_renderer.hpp"

// Currently based on Drawable can we inherit????

TextRenderer::TextRenderer() : 
    texture{},
    shader(".//shaders//text.vert", ".//shaders//text.frag")
{
    setUpBuffers();
    shader.useProgram();
    uniformModelTrans = shader.getUniformLocation("model");
    if (!uniformModelTrans)
       throw "Failed to get get location of uniform \'model\'";

    uniformProjTrans = shader.getUniformLocation("projection");
    if (!uniformProjTrans)
        throw "Failed to get get location of uniform \'projection\'";

    // Set orthogonal projection matrix
    glm::mat4 projection = glm::ortho(0.0f, 640.0f,  480.0f, 0.0f, -1.0f, 1.0f); // diff
    //glm::mat4 projection = glm::mat4(1.0f);
    glUniformMatrix4fv(uniformProjTrans, 1, GL_FALSE, glm::value_ptr(projection));

    // Create transformation matrix
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(320.0f, 240.0f, 0.0f));
    // trans = glm::rotate(trans, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(100, 100, 100));

    glUniformMatrix4fv(uniformModelTrans, 1, GL_FALSE, glm::value_ptr(trans));
}

TextRenderer::~TextRenderer(){
    releaseBuffers();
}


void TextRenderer::setUpBuffers(){
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
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(2*sizeof(float)));

    // Bind EBO and copy element data into EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size()*sizeof(GLuint), elements.data(), GL_STATIC_DRAW);

    // Unbind VAO
    glBindVertexArray(0);
}

void TextRenderer::releaseBuffers(){
    // Delete buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void TextRenderer::draw(){

    texture.bind();
    shader.useProgram();
    glBindVertexArray(VAO);
    glDrawElements(drawingMode, elements.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    texture.unbind();
}
