#include "ray.h"

void Ray::genVertexData(float drawStep) {
	// initialize (if necessary)

	if (this->VAO == 0)
	{
		//cout << this->prevTMax << " " <<this->tMax << endl;
		this->vertexData.clear();
		this->vertexData.push_back(this->origin.x);
		this->vertexData.push_back(this->origin.y);
		this->vertexData.push_back(this->origin.z);
		glm::vec3 end = this->origin + this->dir * this->tMax * drawStep;
		//cout << Util::vec3ToString(this->origin) << " vs " << Util::vec3ToString(end) << endl;
		this->vertexData.push_back(end.x);
		this->vertexData.push_back(end.y);
		this->vertexData.push_back(end.z);
		// color
		glm::vec3 normColor = this->color;
		this->vertexData.push_back(normColor.x);
		this->vertexData.push_back(normColor.y);
		this->vertexData.push_back(normColor.z);
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertexData.size(), this->vertexData.data(), GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(this->VAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}
}

void Ray::draw(const glm::mat4& projection, const glm::mat4& view, float drawStep, float trans) {
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

glm::vec3 Ray::refractRay(glm::vec3& i, glm::vec3& n, float inIndex, float outIndex) {
	//https://physics.stackexchange.com/questions/435512/snells-law-in-vector-form
	glm::vec3 nn = -n;
	float u = inIndex / outIndex;
	
	float ni = glm::dot(nn, i);
	glm::vec3 t = nn * sqrtf(1 - pow(u, 2) * (1 - pow(ni, 2))) + u * (i - ni * nn);

	return glm::normalize(t);
}


void Ray::updateVAO(float drawStep) {
	// reset VAO
	this->VAO = 0;
	this->genVertexData(drawStep);
}
void Ray::setTMax(glm::vec3 rayEnd) {
	this->tMax = glm::length(this->origin- rayEnd);
}

void Ray::setTMax(float tMax) {
	this->tMax = tMax;
}

void Ray::setColor(glm::vec3 color) {
	this->color = color;

	/*if (updateVAO)
		this->updateVAO(drawStep);*/
}