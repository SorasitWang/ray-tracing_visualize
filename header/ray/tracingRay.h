#ifndef TRACINGRAY_H
#define TRACINGRAY_H

#include "ray.h"
#include "../object/object.h"
using namespace std;
class TracingRay : Ray
{
public:
	
	TracingRay* parent;
	vector<TracingRay*> children;
	vector<Object*>* objects;
	float coeff;
	int updated,level;
	glm::vec3 color;
	~TracingRay() {};
	TracingRay(TracingRay* parent, int leftLevel,float coeff,glm::vec3 rayOrigin, glm::vec3 rayDir):Ray(rayOrigin,rayDir) {
		this->parent = parent;
		this->coeff = coeff;
		this->updated = 0;
		this->level = leftLevel;
	}
	void updateValue(glm::vec3 childColor) {
		this->updated += 1;
		this->color += childColor;
		if (this->updated == this->children.size())
			this->parent->updateValue(this->color*this->coeff);
	}

	void extract(glm::vec3 origin,glm::vec3 dir) {
		if (this->level == 0)
			return;
		this->children.push_back(new TracingRay(this,this->level-1, 1,origin,dir));
	}

	void trace() {
	
	}
};

#endif 