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

class Ray
{
public :
	glm::vec3 origin, dir;
	float tMax;
	Ray() {};
	~Ray() {};
	Ray(glm::vec3 rayOrigin, glm::vec3 rayDir) {
		this->origin = rayOrigin;
		this->dir = glm::normalize(rayDir);
	}
	void setTMax(glm::vec3 rayEnd) {
		this->tMax = glm::distance(this->origin, rayEnd);
	}
};

#endif 