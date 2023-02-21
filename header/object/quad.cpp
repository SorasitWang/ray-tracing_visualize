#include "quad.h"

void Quad::genVertexData() {

    if (this->VAO == 0)
    {
        this->vertexData = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)* this->vertexData.size(), this->vertexData.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    this->updateAABB();
  

}

void Quad::draw(const glm::mat4& projection, const glm::mat4& view) {
    this->shader->use();
    this->shader->setMat4("view", view);
    this->shader->setMat4("projection", projection);
    this->shader->setMat4("model", glm::mat4(1.0f));
    this->shader->setVec3("color", glm::vec3(1, 0, 0));
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
bool Quad::isIntersect(Ray ray) {
    
    ray.origin = glm::vec3(this->invTrs * glm::vec4(ray.origin, 1.0f));
    ray.dir = glm::vec3(this->invTrs * glm::vec4(ray.dir, 1.0f));
    float tNear, tFar;
    // tNear and tFar may too close
    if (!this->isIntersectAABB(ray, tNear, tFar)) return false;

    return true;
    
}