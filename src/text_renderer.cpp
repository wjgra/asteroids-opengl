#include "../include/text_renderer.hpp"

// Currently based on Drawable can we inherit????

TextRenderer::TextRenderer() : 
    shader(".//shaders//text.vert", ".//shaders//text.frag"),
    texture{}
{
    setUpBuffers();
    shader.useProgram();
    uniformModelTrans = shader.getUniformLocation("model");
    if (uniformModelTrans < 0)
       throw "Failed to get get location of uniform \'model\'";

    uniformProjTrans = shader.getUniformLocation("projection");
    if (uniformProjTrans < 0)
        throw "Failed to get get location of uniform \'projection\'";

    // Set orthogonal projection matrix
    glm::mat4 projection = glm::ortho(0.0f, 640.0f,  480.0f, 0.0f, -1.0f, 1.0f); // diff
    //glm::mat4 projection = glm::mat4(1.0f);
    glUniformMatrix4fv(uniformProjTrans, 1, GL_FALSE, glm::value_ptr(projection));


    setPosition(100.0f, 320.0f, 240.0f);
    /*
    // Create transformation matrix
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(320.0f, 240.0f, 0.0f));
    // trans = glm::rotate(trans, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(100, 100, 1));

    glUniformMatrix4fv(uniformModelTrans, 1, GL_FALSE, glm::value_ptr(trans));*/

    uniformTextureCoordOffset = shader.getUniformLocation("textureCoordOffset");
    if (uniformTextureCoordOffset < 0)
        throw "Failed to get get location of uniform \'uniformTextureCoordOffset\'";

    //glUniform2f(uniformTextureCoordOffset, 1*0.125f, 4*0.125f);
    //glUniform2f(uniformTextureCoordOffset, 0.0f, 0.0f);
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));

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

/*void TextRenderer::draw(){

    texture.bind();
    shader.useProgram();
    setChar('W');
    //glUniform2f(uniformTextureCoordOffset, 0.125f, 0.125f);

    glBindVertexArray(VAO);
    glDrawElements(drawingMode, elements.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    texture.unbind();
}*/

// Limited to uppercase letters, numbers and common symbols - trivial to extend to other ASCII characters
void TextRenderer::setChar(char toDraw){
    if (toDraw < ' ' || toDraw > '_'){
        glUniform2f(uniformTextureCoordOffset, 0.0f, 0.0f); // Draw space if out of range
    }
    else{
        int xOffset = (toDraw - ' ') % 8;
        int yOffset = (toDraw - ' ') / 8;
        glUniform2f(uniformTextureCoordOffset, xOffset*0.125f, yOffset*0.125f);
    }
    
}

void TextRenderer::setPosition(float scale, float xPos, float yPos){
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(xPos, yPos, 0.0f));
    trans = glm::scale(trans, glm::vec3(scale, scale, 1));
    glUniformMatrix4fv(uniformModelTrans, 1, GL_FALSE, glm::value_ptr(trans));
}

void TextRenderer::drawString(std::string toDraw, float scale, float xPos, float yPos){
    texture.bind();
    shader.useProgram();
    glBindVertexArray(VAO);
    setPosition(scale, xPos, yPos);
    for (char& ch : toDraw){
        setChar(ch);
        glDrawElements(drawingMode, elements.size(), GL_UNSIGNED_INT, 0);
        xPos += scale;
        setPosition(scale, xPos, yPos);
    }
    glBindVertexArray(0);
    texture.unbind();
}

void TextRenderer::drawStringCentred(std::string toDraw, float scale, float xPos, float yPos){
    drawString(toDraw, scale, xPos-0.5f*toDraw.length()*scale, yPos);
}