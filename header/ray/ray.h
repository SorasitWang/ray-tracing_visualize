#ifndef RAY_H
#define RAY_H
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <string>
#include "../shader.h"
#include "../util.h"
using namespace std;
class Ray
{
public :
	glm::vec3 origin, dir;
	float tMax, drawStep,lineWidth;
	unsigned int VAO, VBO;
	Shader* shader;
	vector<float> vertexData;
	glm::vec3 color;
	~Ray() {
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
	};
	Ray(glm::vec3 rayOrigin, glm::vec3 rayDir) {
		this->origin = rayOrigin;
		this->dir = glm::normalize(rayDir);
		this->shader = new Shader("./header/shader/line.vs", "./header/shader/line.fs", "./header/shader/line.gs");
		this->VAO = 0;
		this->tMax = 10.0f;
		this->color = glm::vec3(1.0f);
		this->drawStep = 0.0f;
		this->lineWidth = 0.04f;
		this->genVertexData(this->drawStep);
		
		
	}

	void updateVAO(float drawStep);
	void setTMax(glm::vec3 rayEnd);

	void setTMax(float tMax);

	void setColor(glm::vec3 color);
	void genVertexData(float drawStep);
	void draw(const glm::mat4& projection, const glm::mat4& view, float drawStep, float trans = 1.0f);


	static glm::vec3 refractRay(glm::vec3& i, glm::vec3& n, float inIndex, float outIndex);

};

#endif 