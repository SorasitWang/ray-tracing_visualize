#include "sphere.h"


void Sphere::buildVerticesSmooth()
{
    //http://www.songho.ca/opengl/gl_sphere.html
    const float PI = acos(-1);

    this->vertices = vector<float>();
    this->normals = vector<float>();
    this->texCoords = vector<float>();
    this->indices = vector<int>();

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / this->radius;    // normal
    float s, t;                                     // texCoord

    float sectorStep = 2 * PI / this->sectorCount;
    float stackStep = PI / this->stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= this->stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = this->radius * cosf(stackAngle);             // r * cos(u)
        z = this->radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= this->sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            this->vertices.push_back(x);
            this->vertices.push_back(y);
            this->vertices.push_back(z);

            // normalized vertex normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            this->normals.push_back(nx);
            this->normals.push_back(ny);
            this->normals.push_back(nz);

            // vertex tex coord between [0, 1]
            s = (float)j / this->sectorCount;
            t = (float)i / this->stackCount;
            this->texCoords.push_back(s);
            this->texCoords.push_back(t);
        }
    }

    // indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    unsigned int k1, k2;
    for (int i = 0; i < this->stackCount; ++i)
    {
        k1 = i * (this->sectorCount + 1);     // beginning of current stack
        k2 = k1 + this->sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < this->sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding 1st and last stacks
            if (i != 0)
            {
                this->indices.push_back(k1);
                this->indices.push_back(k2); 
                this->indices.push_back(k1+1);   // k1---k2---k1+1
            }

            if (i != (this->stackCount - 1))
            {
                this->indices.push_back(k1+1);
                this->indices.push_back(k2);
                this->indices.push_back(k2 + 1);   // k1+1---k2---k2+1
            }
        }
    }

    // generate interleaved vertex array as well
    this->concatAllVertexData();
}

void Sphere::concatAllVertexData() {
    this->vertexData = vector<float>();

    std::size_t i, j;
    std::size_t count = vertices.size();
    for (i = 0, j = 0; i < count; i += 3, j += 2)
    {
        this->vertexData.push_back(vertices[i]);
        this->vertexData.push_back(vertices[i + 1]);
        this->vertexData.push_back(vertices[i + 2]);

        this->vertexData.push_back(normals[i]);
        this->vertexData.push_back(normals[i + 1]);
        this->vertexData.push_back(normals[i + 2]);

        this->vertexData.push_back(texCoords[j]);
        this->vertexData.push_back(texCoords[j + 1]);
    }
};
void Sphere::genVertexData() {

    // initialize (if necessary)
    if (this->VAO == 0)
    {
        this->buildVerticesSmooth();
    };
    this->updateAABB();
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    // fill buffer
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertexData.size()*sizeof(float), this->vertexData.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);   // for index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
        this->indices.size()*sizeof(float),             // data size, # of bytes
        this->indices.data(),               // ptr to index data
        GL_STATIC_DRAW);
    
    // link vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}
void Sphere::draw(const glm::mat4& projection, const glm::mat4& view) {
    // render Cube
    this->shader->use();
    this->shader->setMat4("view", view);
    this->shader->setMat4("projection", projection);
    this->shader->setMat4("model", glm::mat4(1.0f));
    this->shader->setVec3("color", glm::vec3(1, 0, 0));
    glBindVertexArray(this->VAO);
    // draw a sphere with VBO
    glDrawElements(GL_TRIANGLES,this->indices.size(),GL_UNSIGNED_INT,(void*)0);
    glBindVertexArray(0);
}

float Sphere::isIntersect(Ray ray) {
    ray.origin = glm::vec3(this->invTrs * glm::vec4(ray.origin, 1.0f));
    ray.dir = glm::vec3(this->invTrs * glm::vec4(ray.dir, 1.0f));

    float tNear, tFar;
    // Since Cube is equal to AABB
    if (!this->isIntersectAABB(ray, tNear, tFar)) return false;

    return true;
}