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
	vector<TracingRay*> children;
	vector<Object*>* objects;
	int updated, level;
	bool isIntersect, isLightRay;
	~TracingRay() {};
	TracingRay(TracingRay* parent, int leftLevel, glm::vec3 rayOrigin, glm::vec3 rayDir) :Ray(rayOrigin, rayDir) {
		this->parent = parent;
		this->updated = 0;
		this->level = leftLevel;
		this->isIntersect = false;
	}

	void extract(glm::vec3 origin, glm::vec3 dir) {
		if (this->level == 0)
			return;
		this->children.push_back(new TracingRay(this, this->level - 1, origin, dir));
	}

	bool canReach(Ray& ray, const vector<Object*>& objects, glm::vec3& terminal,Object* ignoreObj) {
		float terminalDist = glm::length(terminal - ray.origin);
		for (Object* obj : objects) {
			if (obj == ignoreObj)continue;
			float dis;
			glm::vec3 tmpNormal;
			// ignore object that it intersect
		
			if (obj->isIntersect(ray, dis, tmpNormal) && abs(dis) < terminalDist ) {
				return false;
			}
		}
		// have to set new color anyway, so don't update vao yet
		ray.setTMax(terminal,false);

		return true;
	}

	glm::vec3 trace(const vector<Object*>& objects, const vector<PointLight*>& lights,glm::vec3 viewPos,const glm::mat4& projection,const glm::mat4& view) {
		this->color = glm::vec3(0);
		glm::vec3 reflectColor(0.0f), reFracColor(0.0f);
		float minDis = 1e3;
		Object* intersected = nullptr;
		glm::vec3 normal;
		for (Object* obj : objects) {
			float dis = 0;
			glm::vec3 tmpNormal;
			if (obj->isIntersect(*this, dis, tmpNormal) && dis < minDis) {
				minDis = dis;
				intersected = obj;
				normal = tmpNormal;
			}
		}
		if (intersected == nullptr) {
			
		}
		else {
			this->isIntersect = true;
			glm::vec3 intersectedPoint = this->origin + this->dir * minDis;
			// trace more rays		
			if (this->level != 0) {
				if (intersected->material.reflectCoeff != 0.0f) {
					glm::vec3 reflectDir = Ray::reflectRay(this->dir, normal);
					TracingRay* relfectRay = new TracingRay(this, this->level - 1, intersectedPoint, reflectDir);
					reflectColor = relfectRay->trace(objects, lights, viewPos, projection, view);
				}
				if (intersected->material.refractCoeff != 0.0f) {
					// air index is 1
					float inAngle = glm::acos(abs(glm::dot(this->dir, normal)));
					float outAngle = Ray::refractAngle(inAngle, 1,intersected->material.index);
					//glm::vec3 refractDir = 
					//TracingRay* refractRay = new TracingRay(this, this->level - 1, intersectedPoint, refractDir);
					//reflectColor = refractRay->trace(objects, lights, viewPos, projection, view);
				}
			}

			// trace light ray
			for (PointLight* light : lights) {
				Ray lightRay = Ray(intersectedPoint, light->position - intersectedPoint);
				if (this->canReach(lightRay,objects,light->position,intersected)){
					glm::vec3 colorFromLight = computePhong(*light,*intersected, normal,intersectedPoint, viewPos-intersectedPoint);
					this->color += colorFromLight;
					lightRay.setColor(colorFromLight);
				}
				else {
					lightRay.setColor(glm::vec3(0.3f));
				}
				lightRay.draw(projection, view, 0.5f);
				
			}
		}
		this->setTMax(minDis);
		this->draw(projection,view);
		return this->color;
	}

	glm::vec3 computePhong(const PointLight& light,const Object& object,glm::vec3 normal,glm::vec3 fragPos,glm::vec3 viewDir) {

		glm::vec3 lightDir = normalize(light.position - fragPos);
		// diffuse shading
		float diff = std::max(glm::dot(normal, lightDir), 0.0f);
		// specular shading
		glm::vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(std::max(dot(viewDir, reflectDir), 0.0f), object.material.shininess);
		// attenuation
		float distance = length(light.position - fragPos);
		float attenuation = 1.0 / (light.pointLightProp.constant + light.pointLightProp.linear * distance +
			light.pointLightProp.quadratic * (distance * distance));
		// combine results
		glm::vec3 ambient = light.prop.ambient * object.material.ambient;
		glm::vec3 diffuse = light.prop.diffuse * diff * object.material.diffuse;
		glm::vec3 specular = light.prop.specular * spec * object.material.specular;
		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;
		return (ambient + diffuse + specular);
		
	}
};

#endif 