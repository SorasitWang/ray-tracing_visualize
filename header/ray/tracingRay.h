#ifndef TRACINGRAY_H
#define TRACINGRAY_H

#include "ray.h"
#include "../object/object.h"
#include "../light/pointLight.h"
using namespace std;
class TracingRay : public Ray
{
public:

	TracingRay* parent;
	int updated, level;
	bool isIntersect, isLightRay;
	float mediumIndex;
	~TracingRay() {
		delete parent;
	};
	TracingRay(TracingRay* parent, int level, glm::vec3 rayOrigin, glm::vec3 rayDir,float index=1.0f) :Ray(rayOrigin, rayDir) {
		this->parent = parent;
		this->updated = 0;
		this->level = level;
		this->isIntersect = false;
		this->mediumIndex = index;
	}
	void draw(const glm::mat4& projection, const glm::mat4& view, float step, float trans=1.0f);
	bool canReach(Ray& ray, const vector<Object*>& objects, glm::vec3& terminal, Object* ignoreObj);
	glm::vec3 trace(const vector<Object*>& objects, const vector<PointLight*>& lights,glm::vec3& viewPos,const glm::mat4& projection,const glm::mat4& view,float step) ;
	glm::vec3 computePhong(const PointLight& light, const Object& object, glm::vec3 normal, glm::vec3 fragPos, glm::vec3 viewDir);

private :
	static int MAX_TRACING;
};

#endif 