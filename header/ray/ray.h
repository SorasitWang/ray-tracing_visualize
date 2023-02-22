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
class Ray
{
public :
	glm::vec3 origin, dir;
	float tMax;
	unsigned int VAO, VBO;
	Shader* shader;
	vector<float> vertexData;
	glm::vec3 color;
	~Ray() {};
	Ray(glm::vec3 rayOrigin, glm::vec3 rayDir) {
		this->origin = rayOrigin;
		this->dir = glm::normalize(rayDir);
		this->shader = new Shader("./header/shader/line.vs", "./header/shader/line.fs", "./header/shader/line.gs");
		this->VAO = 0;
		this->tMax = 50.0f;
		this->color = glm::vec3(1.0f);
		this->genVertexData();
		
	}

	void updateVAO() {
		// reset VAO
		this->VAO = 0;
		this->genVertexData();
	}
	void setTMax(glm::vec3 rayEnd, bool updateVAO = true) {
		this->tMax = glm::distance(this->origin, rayEnd);
		//cout << this->tMax << endl;
		if (updateVAO) 
			this->updateVAO();
		
	}

	void setTMax(float tMax,bool updateVAO = true) {
		this->tMax = tMax;
		
		if (updateVAO)
			this->updateVAO();
		
	}

	void setColor(glm::vec3 color,bool updateVAO=true) {
		this->color = color;

		if (updateVAO)
			this->updateVAO();
	}
	void genVertexData() {
		// initialize (if necessary)
		
		if (this->VAO == 0)
		{
			this->vertexData.clear();
			this->vertexData.push_back(this->origin.x);
			this->vertexData.push_back(this->origin.y);
			this->vertexData.push_back(this->origin.z);
			glm::vec3 end = this->origin + this->dir * this->tMax;
			
			this->vertexData.push_back(end.x);
			this->vertexData.push_back(end.y);
			this->vertexData.push_back(end.z);
			// color
			this->vertexData.push_back(this->color.x);
			this->vertexData.push_back(this->color.y);
			this->vertexData.push_back(this->color.z);
			glGenVertexArrays(1, &this->VAO);
			glGenBuffers(1, &this->VBO);
			// fill buffer
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertexData.size(), this->vertexData.data(), GL_STATIC_DRAW);
			// link vertex attributes
			glBindVertexArray(this->VAO);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9* sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
	}
	void draw(const glm::mat4& projection, const glm::mat4& view,float trans=1.0f) {
		// render Cube
		this->shader->use();
		this->shader->setMat4("view", view);
		this->shader->setMat4("projection", projection);
		this->shader->setFloat("transparency", trans);
		glBindVertexArray(this->VAO);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	}

	static glm::vec3 reflectRay(glm::vec3 in, glm::vec3 normal) {
		return in - 2.0f * normal * glm::dot(in, normal);
	};

	static float refractAngle(float inAngle, float inIndex, float outIndex) {
		return glm::asin(inIndex * glm::sin(inAngle) /outIndex);
	}
};

#endif 