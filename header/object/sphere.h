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
	~Sphere() {};
	virtual bool isIntersect(Ray ray, float& distance);
	void draw(const glm::mat4& projection, const glm::mat4& view);
	string hello() {
		return "sphere";
	}
private:
	float radius;
	unsigned int stackCount, sectorCount, EBO;
	vector<float> vertices, normals, texCoords, vertexData;
	vector<int> indices;
	virtual void genVertexData();
	void buildVerticesSmooth();
	void concatAllVertexData();

};

#endif // !SPHERE_H
