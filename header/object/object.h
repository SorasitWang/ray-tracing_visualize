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
#include "../light/pointLight.h"
using namespace std;

struct Material
{
	glm::vec3 ambient = glm::vec3(0.4f,0.1f,0.1f);
	glm::vec3 diffuse = glm::vec3(0.4f, 0.1f, 0.1f);
	glm::vec3 specular = glm::vec3(0.4f, 0.1f, 0.1f);
	int shininess = 1;
	float index = 1.0f;
	float reflectCoeff = 0.0f;
	float refractCoeff = 0.0f;
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
		this->shader = new Shader("./header/shader/phong.vs", "./header/shader/phong.fs");
		//this->shader = new Shader("./header/shader/plainColor.vs", "./header/shader/plainColor.fs");
		this->updateTransformMatrix(glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f));

	};
	~Object() {
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
	};
	vector<float> vertexData;
	vector<glm::vec3> vertexWorldPos;


	void updateTransformMatrix(glm::mat4 t,glm::mat4 r,glm::mat4 s);
	
	bool isIntersectAABB(Ray ray, float& tNear, float& tFar);
	void setMaterial(string type, glm::vec3 val);
	void setMaterial(string type, float val);
	void setPhongUniform(const vector<PointLight*>* pointLights, const glm::vec3& viewPos, const glm::mat4& projection, const glm::mat4& view);
	bool isIntersectFiltered(Ray ray, float& distance, glm::vec3& normal);
	virtual bool isIntersect(Ray ray,float& tNear, float& tFar, glm::vec3& normal) = 0;
	void drawPhong(const vector<PointLight*>* pointLights, const glm::vec3& viewPos, const glm::mat4& projection, const glm::mat4& view);
	virtual void draw()=0;
	virtual string hello()=0;
	void drawDepth(const PointLight* light);
private :

protected:
	Shader* shader;
	void updateAABB();
	bool isInsideAABB(glm::vec3 pos);
	virtual void genVertexData() = 0;
	
	
};
#endif //!OBJECT_H
