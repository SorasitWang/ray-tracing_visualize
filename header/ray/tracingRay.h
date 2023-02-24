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
	float mediumIndex;
	~TracingRay() {};
	TracingRay(TracingRay* parent, int leftLevel, glm::vec3 rayOrigin, glm::vec3 rayDir,float index=1.0f) :Ray(rayOrigin, rayDir) {
		this->parent = parent;
		this->updated = 0;
		this->level = leftLevel;
		this->isIntersect = false;
		this->mediumIndex = index;
	}

	void extract(glm::vec3 origin, glm::vec3 dir) {
		if (this->level == 0)
			return;
		this->children.push_back(new TracingRay(this, this->level - 1, origin, dir));
	}

	bool canReach(Ray& ray, const vector<Object*>& objects, glm::vec3& terminal,Object* ignoreObj) {
		float terminalDist = glm::length(terminal - ray.origin);
		for (Object* obj : objects) {
			//if (obj == ignoreObj)continue;
			float tNear,tFar;
			glm::vec3 tmpNormal;
			// ignore object that it intersect
		
			if (obj->isIntersect(ray, tNear, tFar, tmpNormal) && abs(tNear) < terminalDist && tNear > 0) {
				return false;
			}
			//cout << tNear << " " <<tFar << endl;
		}
		// have to set new color anyway, so don't update vao yet
		ray.setTMax(terminal,false);

		return true;
	}

	glm::vec3 trace(const vector<Object*>& objects, const vector<PointLight*>& lights,glm::vec3& viewPos,const glm::mat4& projection,const glm::mat4& view) {
		
		glm::vec3 localColor(0.0f),reflectColor(0.0f), refractColor(0.0f);
		float minDis = 1e3;
		Object* intersected = nullptr;
		bool underShadow = true;
		glm::vec3 normal;
		this->color = glm::vec3(0.0f);
		for (Object* obj : objects) {
			float tNear=0,tFar=0;
			glm::vec3 tmpNormal;
			if (obj->isIntersect(*this, tNear,tFar, tmpNormal) ) {
				// from surface to outside or to inside

				if (abs(tNear) < 1e-4 || abs(tFar) < 1e-4 ) {
					if (abs(tFar) < 1e-4) {
						// to outside
						continue;
						
					}
					// inside
					else {
						minDis = tFar;
					}
					
				}
				else
				{
					minDis = tNear;
				}
				intersected = obj;
				normal = tmpNormal;
				//cout << Util::vec3ToString(this->origin + this->dir * tNear) << " " << Util::vec3ToString(this->dir) << endl;		
			}
			
		}
		if (intersected == nullptr) {
			this->color = glm::vec3(1);
		}
		else {
			this->isIntersect = true;
			glm::vec3 intersectedPoint = this->origin + this->dir * minDis;
			// trace more rays		
			if (this->level > 0) {
				if (intersected->material.reflectCoeff != 0.0f) {
					glm::vec3 reflectDir = glm::reflect(this->dir, normal);
					TracingRay* relfectRay = new TracingRay(this, this->level - 1, intersectedPoint, reflectDir);
					reflectColor = relfectRay->trace(objects, lights, viewPos, projection, view);
					//cout << Util::vec3ToString(reflectDir) << " | " << Util::vec3ToString(intersectedPoint)<< endl;
				}
				if (intersected->material.refractCoeff != 0.0f) {
					// air index is 1
					float index = intersected->material.index;
					if (this->mediumIndex == intersected->material.index) {
						// ray is coming from inside
						index = 1.0f;
					}
					glm::vec3 refractDir = -1.0f * Ray::refractRay(this->dir, normal, this->mediumIndex, index);
					TracingRay* refractRay = new TracingRay(this, this->level - 1, intersectedPoint, refractDir, intersected->material.index);
					refractColor = refractRay->trace(objects, lights, viewPos, projection, view);
					cout << Util::vec3ToString(refractDir) << " " << Util::vec3ToString(intersectedPoint) << " " << refractRay->tMax << endl;
				}
			}

			// trace light ray
			for (PointLight* light : lights) {
				Ray lightRay = Ray(intersectedPoint, light->position - intersectedPoint);
				if (this->canReach(lightRay,objects,light->position,intersected)){
					underShadow = false;
					glm::vec3 colorFromLight = computePhong(*light,*intersected, normal,intersectedPoint, viewPos-intersectedPoint);
					localColor += colorFromLight;
					lightRay.setColor(colorFromLight);
				}
				else {
					lightRay.setColor(glm::vec3(0.3f));
				}
				lightRay.draw(projection, view, 0.5f);
				
			}
			if (underShadow) {
				this->color = glm::vec3(0.0f);
			}
			else {
				this->color += localColor * (1 - min(1.0f, intersected->material.refractCoeff + intersected->material.reflectCoeff));
				this->color += refractColor * intersected->material.refractCoeff;
				this->color += reflectColor * intersected->material.reflectCoeff;
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
		//cout << Util::vec3ToString(ambient) << "/" << Util::vec3ToString(diffuse) << "/" << Util::vec3ToString(specular) << endl;
		return (ambient + diffuse + specular);
		
	}
};

#endif 