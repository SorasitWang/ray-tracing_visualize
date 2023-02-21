#ifndef QUAD_H
#define QUAD_H

#include "object.h"

class Quad : private Object {

public:
	Quad() : Object() {
		this->genVertexData();
	};
	~Quad() {};
	virtual bool isIntersect(Ray ray, float& distance);
	void draw(const glm::mat4& projection, const glm::mat4& view);
	string hello() {
		return "quad";
	}

private:
	virtual void genVertexData();
};

#endif // !QUAD_H
