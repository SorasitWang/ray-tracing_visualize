
#include "pointLight.h"
// initialize (if necessary)
void PointLight::genVAO() {
    if (this->VAO == 0)
    {
        vector<float> vertices = Util::getCubeVertexData();
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(this->VAO);
        glEnableVertexAttribArray(0);
        // just position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void PointLight::draw(const glm::mat4& projection, const glm::mat4& view) {
    this->shader->use();
    this->shader->setVec3("color", this->prop.diffuse);
    this->shader->setMat4("model", glm::translate(glm::mat4(1.0f), this->position));
    this->shader->setMat4("projection", projection);
    this->shader->setMat4("view", view);
    // render Cube
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}