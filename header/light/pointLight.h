#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <string>
#include "../shader.h"
#include "./light.h"
using namespace std;

struct PointLightProp {
	float constant=1.0f;
	float linear = 1.0f;
	float quadratic = 1.0f;
};
class PointLight : public Light
{
public:
	PointLightProp pointLightProp;
	glm::vec3 position;
	PointLight(glm::vec3 pos) : Light() {
		this->position = pos;
		shader = new Shader("./header/shader/plainColor.vs", "./header/shader/plainColor.fs");
		this->genVAO();
		this->setPointLightProp(1, 1, 1);
	};
	~PointLight() {};

	void setPointLightProp(float constant,float linear,float quadratic) {
		this->pointLightProp.constant = constant;
		this->pointLightProp.linear = linear;
		this->pointLightProp.quadratic = quadratic;
	}
	
	void draw(const glm::mat4& projection, const glm::mat4& view);
private:
	unsigned int VAO, VBO;
	Shader* shader;
	

	void genVAO();
};
#endif //!POINTLIGHT_H
