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

void Object::setMaterial(string type,glm::vec3 val) {
	if (type == "ambient") {
		this->material.ambient = val;
	}
	else if (type == "diffuse") {
		this->material.diffuse = val;
	}
	else if (type == "specular") {
		this->material.ambient = val;
	}
}
void Object::setMaterial(string type, float val) {
	if (type == "shininess") {
		this->material.shininess = val;
	}
	else if (type == "index") {
		this->material.index = val;
	}
	else if (type == "reflectCoeff") {	
		this->material.reflectCoeff = val;
	}
	else if (type == "refractCoeff") {
		this->material.refractCoeff = val;
	}
	if (this->material.refractCoeff + this->material.refractCoeff > 1.0f)
		cout << "WARNING : reflect + refract coeff should not more than 1" << endl;
}

void Object::setPhongUniform(const vector<PointLight*>* pointLights,const glm::vec3& viewPos,const glm::mat4& projection,const glm::mat4& view) {
	this->shader->use();
	this->shader->setMat4("view", view);
	this->shader->setVec3("viewPos", viewPos);
	this->shader->setInt("numPointLight", pointLights->size());
	for (unsigned int i = 0; i < pointLights->size();i++) {
		PointLight* l = pointLights->at(i);
		string index = "pointLights[" + to_string(i) + "].";
		this->shader->setVec3(index+"ambient",l->prop.ambient);
		this->shader->setVec3(index + "diffuse", l->prop.diffuse);
		this->shader->setVec3(index + "specular", l->prop.specular);

		this->shader->setVec3(index + "position", l->position);

		this->shader->setFloat(index + "constant", l->pointLightProp.constant);
		this->shader->setFloat(index + "linear", l->pointLightProp.linear);
		this->shader->setFloat(index + "quadratic", l->pointLightProp.quadratic);
		// reserved 0-2 for diffuse texture
		this->shader->setInt("shadowMaps[" + to_string(i) + "]", i);
		this->shader->setMat4("lightSpaceMatrixs[" + to_string(i) + "]", l->lightSpace.lightMatrix[0]);
		this->shader->setVec3("lightPos[" + to_string(i) + "]", l->position);
	}
	this->shader->setFloat("farPlane", pointLights->at(0)->farPlane);
	this->shader->setVec3("material.ambient", this->material.ambient);
	this->shader->setVec3("material.diffuse", this->material.diffuse);
	this->shader->setVec3("material.specular", this->material.specular);
	this->shader->setFloat("material.shininess", this->material.shininess);
	this->shader->setMat4("projection", projection);
	this->shader->setMat4("model", this->trs);
}

void Object::drawPhong(const vector<PointLight*>* pointLights, const glm::vec3& viewPos, const glm::mat4& projection, const glm::mat4& view) {
	this->setPhongUniform(pointLights, viewPos, projection, view);
	this->draw();
}

void Object::drawDepth(const PointLight* light) {
		light->depthShader->use();
		light->depthShader->setVec3("lightPos",  light->position);
		light->depthShader->setMat4("model", this->trs);
		for (unsigned int i = 0; i < 6; i++) {
			light->depthShader->setMat4("shadowMatrices["+to_string(i)+"]", light->lightSpace.lightMatrix[i]);
		}
		light->depthShader->setFloat("farPlane", light->farPlane);
		this->draw();
}

bool Object::isIntersectFiltered(Ray ray, float& distance, glm::vec3& normal) {
	glm::vec3 tmpNormal;
	float tNear, tFar;
	if (this->isIntersect(ray, tNear, tFar, tmpNormal)) {
		// from surface to outside or to inside
		if (abs(tNear) < 1e-4 || abs(tFar) < 1e-4) {
			if (abs(tFar) < 1e-4) {
				return false;
			}
			// inside
			else {
				distance = tFar;
			}

		}
		else
		{
			distance = tNear;
		}
		normal = tmpNormal;		
		return true;
	}
	return false;
}