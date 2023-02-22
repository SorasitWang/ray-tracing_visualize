#include "object.h"

void Object::updateAABB() {
	for (unsigned int i = 0; i < this->vertexData.size(); i += 8) {
		this->min = glm::min(this->min, glm::vec3(this->vertexData[i], this->vertexData[i+1], this->vertexData[i+2]));
		this->max = glm::max(this->max, glm::vec3(this->vertexData[i], this->vertexData[i + 1], this->vertexData[i + 2]));
		
	}
}

void Object::updateTransformMatrix(glm::mat4 t, glm::mat4 r, glm::mat4 s) {
	this->t = t;
	this->r = r;
	this->s = s;
	this->trs = (this->t * this->r) * this->s;
	this->invTrs = glm::inverse(this->trs);	
};

bool Object::isIntersectAABB(Ray ray,float &tNear, float &tFar) {
	
	// ray in object space
	// https://gist.github.com/DomNomNom/46bb1ce47f68d255fd5d
	// compute the near and far intersections of the cube (stored in the x and y components) using the slab method
	// no intersection means vec.x > vec.y (really tNear > tFar)
	glm::vec3 tMin = (this->min - ray.origin) / ray.dir;
	glm::vec3 tMax = (this->max - ray.origin) / ray.dir;
	glm::vec3 t1 = glm::min(tMin, tMax);
	glm::vec3 t2 = glm ::max(tMin, tMax);
	tNear = std::max(std::max(t1.x, t1.y), t1.z);
	tFar = std::min(std::min(t2.x, t2.y), t2.z);
	return tNear <= tFar;
}


bool Object::isInsideAABB(glm::vec3 pos)
{
	const float Epsilon = 1e-6;
	if (pos.x > this->min.x - Epsilon && pos.x <  this->max.x + Epsilon &&
		pos.y > this->min.y - Epsilon && pos.y <  this->max.y + Epsilon &&
		pos.z >  this->min.z - Epsilon && pos.z < this->max.z + Epsilon)
	{
		return true;
	}
	return false;
}
