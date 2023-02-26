#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

class Sphere : public Object {

public:

	Sphere(float radius=1.0f,int stackCount=18,int sectorCount=36) : Object() {
		this->radius = radius;
		this->stackCount = stackCount;
		this->sectorCount = sectorCount;
		this->genVertexData();
	};
	~Sphere() {
		glDeleteBuffers(1,&this->EBO);
	};
	virtual bool isIntersect(Ray ray, float& tNear, float& tFar, glm::vec3& normal);
	void draw();
	string hello() {
		return "sphere";
	}
private:
	float radius;
	unsigned int stackCount, sectorCount, EBO;
	vector<float> vertices, normals, texCoords;
	vector<int> indices;
	virtual void genVertexData();
	void buildVerticesSmooth();
	void concatAllVertexData();

};

#endif // !SPHERE_H
