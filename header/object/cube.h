#ifndef CUBE_H
#define CUBE_H

#include "object.h"

class Cube : public Object {

public:
	Cube() : Object() {
		this->genVertexData();
	};
	~Cube() {};
	virtual bool isIntersect(Ray ray, float& distance);
	void draw(const glm::mat4& projection, const glm::mat4& view);
	string hello() {
		return "cube" ;
	}
private :
	virtual void genVertexData();
	
};

#endif // !CUBE_H