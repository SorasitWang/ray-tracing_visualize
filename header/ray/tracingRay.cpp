#include "tracingRay.h"


int TracingRay::MAX_TRACING = 5;
glm::vec3 TracingRay::computePhong(const PointLight& light, const Object& object, glm::vec3 normal, glm::vec3 fragPos, glm::vec3 viewDir) {

	
	glm::vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	//float diff = std::max(glm::dot(normal, lightDir), 0.0f);
	float diff = abs(glm::dot(normal, lightDir));
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

glm::vec3 TracingRay::trace(const vector<Object*>& objects, const vector<PointLight*>& lights, glm::vec3& viewPos, const glm::mat4& projection, const glm::mat4& view, float step) {

	glm::vec3 localColor(0.0f), reflectColor(0.0f), refractColor(0.0f);
	float minDis = 1e3;
	Object* intersected = nullptr;
	bool underShadow = true;
	glm::vec3 normal;
	this->color = glm::vec3(0.0f);
	for (Object* obj : objects) {
		float dis = 0, tFar = 0;
		glm::vec3 tmpNormal;
		if (obj->isIntersect(*this, dis, tmpNormal) && dis < minDis) {
			intersected = obj;
			minDis = dis;
			normal = tmpNormal;
		}
		else {	
		}
	}
		
	if (intersected == nullptr) {
		this->color = glm::vec3(1);
	}
	else {
		
		this->isIntersect = true;
		glm::vec3 intersectedPoint = this->origin + this->dir * minDis;
		//cout << Util::vec3ToString(intersectedPoint) << " , " << Util::vec3ToString(normal) << endl;
		//cout << Util::vec3ToString(intersectedPoint) << endl;
		// trace more rays		
		if (this->level < TracingRay::MAX_TRACING) {
			if (intersected->material.reflectCoeff != 0.0f) {
				glm::vec3 reflectDir = glm::reflect(this->dir, normal);
				TracingRay* relfectRay = new TracingRay(this, this->level + 1, intersectedPoint, reflectDir);
				reflectColor = relfectRay->trace(objects, lights, viewPos, projection, view, step);
				//cout << this->level << " : " <<  Util::vec3ToString(intersectedPoint) << " | " << Util::vec3ToString(reflectDir) << " | " << Util::vec3ToString(normal) << endl;
			}
			if (intersected->material.refractCoeff != 0.0f) {
				// air index is 1
				float index = intersected->material.index;
				if (this->mediumIndex == intersected->material.index) {
					// ray is coming from inside
					index = 1.0f;
				}
				glm::vec3 refractDir = Ray::refractRay(this->dir, normal, this->mediumIndex, index);
				TracingRay* refractRay = new TracingRay(this, this->level + 1, intersectedPoint, refractDir, intersected->material.index);
				refractColor = refractRay->trace(objects, lights, viewPos, projection, view, step);
				//cout << this->level << " : " << Util::vec3ToString(this->dir) << " " << Util::vec3ToString(glm::normalize(refractDir)) << " " << Util::vec3ToString(glm::normalize(normal)) << endl;
			}
		}

		//trace light ray
		for (PointLight* light : lights) {
			TracingRay* lightRay = new TracingRay(this,this->level+1,intersectedPoint, light->position - intersectedPoint);
			//Ray lightRay2 = Ray(lightRay->origin, lightRay->dir);
			if (this->canReach(*lightRay, objects, light->position, intersected)) {
				underShadow = false;
				glm::vec3 colorFromLight = computePhong(*light, *intersected, normal, intersectedPoint, viewPos - intersectedPoint);
				localColor += colorFromLight;
				lightRay->setColor(colorFromLight);
			}
			else {
				lightRay->setColor(glm::vec3(0.3f));
			}
			lightRay->draw(projection, view, step,0.3f);

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
	this->draw(projection, view, step);
	return this->color;
}


bool TracingRay::canReach(Ray& ray, const vector<Object*>& objects, glm::vec3& terminal, Object* ignoreObj) {
	float terminalDist = glm::length(terminal - ray.origin);
	for (Object* obj : objects) {
		//if (obj == ignoreObj)continue;
		float tNear,tFar;
		glm::vec3 tmpNormal;
		// ignore object that it intersect
		if (obj->isIntersect(ray, tNear, tmpNormal))
			return false;
	}
	ray.setTMax(terminal);

	return true;
}

void TracingRay::draw(const glm::mat4& projection, const glm::mat4& view, float step, float trans) {
	float drawStep = 0.0f;
	if (step < this->level)
		drawStep = 0;
	else
		drawStep = min(1.0f, step - this->level);
	// render Cube
	this->shader->use();
	this->shader->setMat4("view", view);
	this->shader->setMat4("projection", projection);
	this->shader->setFloat("transparency", trans);
	this->shader->setFloat("lineWidth", this->lineWidth);
	this->VAO = 0;
	this->genVertexData(drawStep);
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}