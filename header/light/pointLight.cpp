
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
    glm::mat4 model = glm::translate(glm::mat4(1.0f), this->position);
    model = glm::scale(model, glm::vec3(0.1f));
    this->shader->setMat4("model", model);
    this->shader->setMat4("projection", projection);
    this->shader->setMat4("view", view);
    // render Cube
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void PointLight::initDepthMap() {
    // TODO : use cube box instead
    glGenFramebuffers(1, &this->depthMapFBO);
    glGenTextures(1, &this->depthMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->depthMap);
    for (unsigned int i=0;i<6;i++)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_DEPTH_COMPONENT,
            this->shadowW, this->shadowH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
unsigned int PointLight::renderDepthMap() {
    


    this->lightSpace.lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, this->nearPlane, this->farPlane);
    this->lightSpace.lightMatrix[0] = this->lightSpace.lightProjection * glm::lookAt(this->position, this->position + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)) ;
    this->lightSpace.lightMatrix[1] = this->lightSpace.lightProjection * glm::lookAt(this->position, this->position + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0));
    this->lightSpace.lightMatrix[2] = this->lightSpace.lightProjection * glm::lookAt(this->position, this->position + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));
    this->lightSpace.lightMatrix[3] = this->lightSpace.lightProjection * glm::lookAt(this->position, this->position + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1));
    this->lightSpace.lightMatrix[4] = this->lightSpace.lightProjection * glm::lookAt(this->position, this->position + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0));
    this->lightSpace.lightMatrix[5] = this->lightSpace.lightProjection * glm::lookAt(this->position, this->position + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0));

    glViewport(0, 0, this->shadowW, this->shadowH);
    glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);    

    return this->depthMap;
}