#include "quad.h"

void Quad::genVertexData() {

    if (this->VAO == 0)
    {
        this->vertexData = {
            // back face
            -1.0f, -1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            1.0f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            1.0f, -1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
            1.0f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
         
        };
        // setup plane VAO
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)* this->vertexData.size(), this->vertexData.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    this->updateAABB();
  

}

void Quad::draw() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
bool Quad::isIntersect(Ray ray, float& distance, glm::vec3& normal) {
    
    
    Ray objRay(glm::vec3(this->invTrs * glm::vec4(ray.origin, 1.0f)), glm::vec3(this->invTrs * glm::vec4(ray.dir, 0.0f)));

    float _tNear, _tFar;
    this->isIntersectAABB(objRay, _tNear, _tFar);

    // since tNear and tFar may be the same point
    if ( abs(_tFar) > 1e6 || abs(_tNear) > 1e6)
        return false;

    // assuming vectors are all normalized
    glm::vec3 n(0, 0, 1);

    float denom = glm::dot(n, objRay.dir);
    //cout << denom << endl;
    if (abs(denom) > 1e-6) {
        glm::vec3 p0l0 = glm::vec3(0,0,0) - objRay.origin;
        float t = glm::dot(p0l0, n) / denom;
        if (t >= 1e-4 && this->isInsideAABB(objRay.origin + objRay.dir*t)) {
            distance = t;
            normal = glm::normalize(glm::sign(denom) * n);
            normal = glm::mat3(glm::transpose(glm::inverse(this->trs))) * normal;
          
            return true;
        }
    }
   
    return false;
    
}