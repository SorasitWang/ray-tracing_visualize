#ifndef CUBE_H
#define CUBE_H

#include "object.h"

class Cube : public Object {

public:
	Cube() : Object() {
		this->genVertexData();
	};
	~Cube() {};
	virtual bool isIntersect(Ray ray, float& tNear, float& tFar, glm::vec3& normal);
	void draw();
	void drawPhong(const vector<PointLight*>* pointLights, const glm::vec3& viewPos, const glm::mat4& projection, const glm::mat4& view) {
		this->setPhongUniform(pointLights, viewPos, projection, view);
		this->draw();
	}
	string hello() {
		return "cube" ;
	}
private :
	virtual void genVertexData();
	
};

#endif // !CUBE_H
