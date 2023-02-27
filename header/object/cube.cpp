#include "cube.h"

void Cube::genVertexData() {
  
    // initialize (if necessary)
    if (this->VAO == 0)
    {
        this->vertexData = Util::getCubeVertexData();
        cout << this->vertexData.size() << endl;
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)* this->vertexData.size(), this->vertexData.data(), GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(this->VAO);
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
void Cube::draw() {
    // render Cube
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

bool Cube::isIntersect(Ray ray, float& distance, glm::vec3& normal) {
    Ray objRay(glm::vec3(this->invTrs * glm::vec4(ray.origin, 1.0f)), glm::vec3(this->invTrs * glm::vec4(ray.dir, 0.0f)));
    float _tNear, _tFar;
    int signNormal = 1;
    // Since Cube is equal to AABB
    if (!this->isIntersectAABB(objRay,_tNear,_tFar)) return false;
    if (abs(_tNear) < 1e-4 || abs(_tFar) < 1e-4) {
        if (abs(_tFar) < 1e-4) {
            return false;
        }
        // inside
        else {
            distance = _tFar;
            signNormal = -1;
        }
    }
    else
    {
        distance = _tNear;
    }

    glm::vec3 objIntersectPoint = objRay.origin + objRay.dir * distance;
    //cout << objIntersectPoint.x << " " << objIntersectPoint.y << " " << objIntersectPoint.z << endl;
    if (abs(objIntersectPoint.x) - 1 < 1e-4) {
        normal = glm::vec3(glm::sign(objIntersectPoint.x), 0, 0);
    }
    else if (abs(objIntersectPoint.y) - 1 < 1e-4) {
        normal = glm::vec3(0,glm::sign(objIntersectPoint.y), 0);
    }
    else if (abs(objIntersectPoint.z) - 1 < 1e-4) {
        normal = glm::vec3(0,0,glm::sign(objIntersectPoint.z));
    }
    normal *= signNormal;
    normal = glm::mat3(glm::transpose(glm::inverse(this->trs)))* normal;
    
  
    

    return true;
}