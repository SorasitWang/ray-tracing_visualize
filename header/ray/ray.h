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
	float tMax, drawStep;
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
		this->tMax = 10.0f;
		this->color = glm::vec3(1.0f);
		this->drawStep = 0.0f;
		this->genVertexData(this->drawStep);
		
		
	}

	bool inRange(glm::vec3 pos) {
		float value = glm::length((pos - this->origin) / this->dir);
		if (value > this->tMax || value < 0)
			return false;
		return true;
	}
	void updateVAO(float drawStep) {
		// reset VAO
		this->VAO = 0;
		this->genVertexData(drawStep);
	}
	void setTMax(glm::vec3 rayEnd, float drawStep, bool updateVAO = true) {
		this->tMax = glm::distance(this->origin, rayEnd);
		//cout << this->tMax << endl;
	/*	if (updateVAO) 
			this->updateVAO(drawStep);*/
		
	}

	void setTMax(float tMax, bool updateVAO = true) {
		this->tMax = tMax;
		
		/*if (updateVAO)
			this->updateVAO(drawStep);*/
		
	}

	void setColor(glm::vec3 color,bool updateVAO=true) {
		this->color = color;

		/*if (updateVAO)
			this->updateVAO(drawStep);*/
	}
	void genVertexData(float drawStep) {
		// initialize (if necessary)
	
		if (this->VAO == 0)
		{
			//cout << this->prevTMax << " " <<this->tMax << endl;
			this->vertexData.clear();
			this->vertexData.push_back(this->origin.x);
			this->vertexData.push_back(this->origin.y);
			this->vertexData.push_back(this->origin.z);
			glm::vec3 end = this->origin + this->dir * this->tMax*drawStep;
			//cout << Util::vec3ToString(this->origin) << " vs " << Util::vec3ToString(this->dir) << endl;
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
	void draw(const glm::mat4& projection, const glm::mat4& view,float drawStep,float trans=1.0f) {
		// render Cube
		this->shader->use();
		this->shader->setMat4("view", view);
		this->shader->setMat4("projection", projection);
		this->shader->setFloat("transparency", trans);
		this->VAO = 0;
		this->genVertexData(drawStep);
		glBindVertexArray(this->VAO);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	}


	static glm::vec3 refractRay(glm::vec3& i,glm::vec3& n, float inIndex, float outIndex) {
		//https://physics.stackexchange.com/questions/435512/snells-law-in-vector-form
		glm::vec3 nn = -n;
		float u = inIndex / outIndex;
		float ni = glm::dot(nn, i);
		glm::vec3 t = nn * sqrtf(1 - pow(u, 2) * (1 - pow(ni, 2))) + u*(i-ni*nn);
		return glm::normalize(t);
	}

	static glm::vec3 refractRay2(glm::vec3& i, glm::vec3& n, float nIn, float nOut) {
		
		// Calculate the angle of incidence
		glm::vec3 nn = -n;
		float cosi = glm::dot(i,nn);
		cout << cosi << endl;
		float theta_i = acos(cosi);
		float rat = (nIn / nOut);
		// Calculate the angle of refraction using Snell's law
		float sinr = rat * sin(theta_i);
		float thetar = asin(sinr);
		//cout << cosi << " " << theta_i << endl;
		// Calculate the refracted ray vector
	
		glm::vec3 nr = rat * n;
		float cosr = cos(theta_i);
		glm::vec3 ir = rat * i - (rat * cosi + sqrt(1 - sinr * sinr)) * nr;
		//cout << Util::vec3ToString(i) << " " << " " << Util::vec3ToString(n)  << " " << Util::vec3ToString(ir) << " =>" << nIn << " " << nOut << endl;
		return glm::normalize(ir);
	}
};

#endif 