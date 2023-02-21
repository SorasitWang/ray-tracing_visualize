#ifndef LIGHT_H
#define LIGHT_H
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <string>
#include "../util.h"
using namespace std;

struct LightProperty {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};
class Light
{
public:
	
	
	LightProperty prop;
	Light() {
		this->prop = LightProperty();
		this->setLightProp(glm::vec3(0.3f), glm::vec3(0.3f), glm::vec3(0.3f));
		
	};
	~Light() {};
	void setLightProp(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
		this->prop.ambient = ambient;
		this->prop.diffuse = diffuse;
		this->prop.specular = specular;
	}



private:
	

};
#endif //!LIGHT_H