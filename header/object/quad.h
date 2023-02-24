#ifndef QUAD_H
#define QUAD_H

#include "object.h"

class Quad : private Object {

public:
	Quad() : Object() {
		this->genVertexData();
	};
	~Quad() {};
	virtual bool isIntersect(Ray ray, float& tNear,float& tFar, glm::vec3& normal);
	void draw();
	string hello() {
		return "quad";
	}

private:
	virtual void genVertexData();
};

#endif // !QUAD_H
