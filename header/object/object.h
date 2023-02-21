#ifndef OBJECT_H
#define OBJECT_H
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <string>
#include "../util.h"
#include "../ray/ray.h"
#include "../shader.h"
using namespace std;

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	float index;
};
class Object 
{
public :

	unsigned int VAO, VBO;
	glm::vec3 min, max;
	glm::mat4 t, r, s, trs, invTrs;
	Material material;
	Object() {
		this->VAO = 0;
		this->VBO = 0;
		this->max = glm::vec3(-10000);
		this->min = glm::vec3(10000);
		/*this->shader = new Shader("../shader/phong.vs", "../shader/phong.fs");*/
		this->shader = new Shader("./header/shader/plainColor.vs", "./header/shader/plainColor.fs");

	};
	~Object() {};
	vector<float> vertexData;
	vector<glm::vec3> vertexWorldPos;


	void updateTransformMatrix(glm::mat4 t,glm::mat4 r,glm::mat4 s);
	bool isIntersectAABB(Ray ray, float& tNear, float& tFar);

	virtual bool isIntersect(Ray ray,float& distance) = 0;
	virtual void draw(const glm::mat4& projection, const glm::mat4& view)=0;
	virtual string hello()=0;
private :

protected:
	Shader* shader;
	void updateAABB();
	bool isInsideAABB(glm::vec3 pos);
	virtual void genVertexData() = 0;
	
	
};
#endif //!OBJECT_H
