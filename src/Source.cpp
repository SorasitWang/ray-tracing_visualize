
#include <iostream>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <string>
#include "../header/camera.h"
#include "../header/object/cube.h"
#include "../header/object/sphere.h"
#include "../header/object/quad.h"
#include "../header/light/pointLight.h"
#include "../header/ray/tracingRay.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void draw();
void deleteAll();
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float step = 0.0f;
int scene = 2;

glm::vec3 dummyCameraPos;
vector<Object*> objs;
vector<PointLight*> lights;
TracingRay* ray;
vector<unsigned int> depthMaps;
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //camera.Position = glm::vec3(-7.55,1.42,10.93);
    //camera.ProcessMouseMovement(400, 0);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
    // normal scene
    if (scene == 0) {
        dummyCameraPos = glm::vec3(3, 0.5, 0);

        Cube* cube = new Cube();
        Cube* cube2 = new Cube();
        Sphere* sphere = new Sphere();
        PointLight* pointLight = new PointLight(glm::vec3(6));
        PointLight* pointLight2 = new PointLight(glm::vec3(3, 2, 0));
        pointLight2->setLightProp(glm::vec3(10, 20, 10), glm::vec3(50, 100, 50), glm::vec3(10, 20, 10));
        glm::mat4 t = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1.5, 0));
        glm::mat4 r = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0, 1, 1));
        glm::mat4 s(1.0f);
        //cube->updateTransformMatrix(t, r, s);
        cube->setMaterial("reflectCoeff", 0.3f);
        cube->setMaterial("index", 1.9f);
        cube->setMaterial("refractCoeff", 0.3f);
        t = glm::translate(glm::mat4(1.0f), pointLight->position);
        cube2->updateTransformMatrix(t, glm::mat4(1.0f), glm::mat4(1.0f));

        objs.push_back(cube);
        //objs.push_back(cube2);
        // 
        //lights.push_back(pointLight);
        lights.push_back(pointLight2);
        ray = new TracingRay(nullptr, 0, dummyCameraPos, glm::vec3(-1, -0.1f, 0));
    }
    else if (scene == 1) {
        dummyCameraPos = glm::vec3(3, 0, 0);

        Quad* mir1 = new Quad();
        mir1->setMaterial("reflectCoeff", 0.75f);
        mir1->updateTransformMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0,0,-0.)), glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0, 1, 0)), glm::mat4(1.0f));
        objs.push_back(mir1);
        
        Quad* mir2 = new Quad();
        mir2->setMaterial("reflectCoeff", 0.75f);
        mir2->updateTransformMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-0.25f, 0, 4)), glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0, 1, 0)), glm::mat4(1.0f));
        mir2->setMaterial("all", glm::vec3(0.0f, 0.6f, 0.9f));
        objs.push_back(mir2);

        Quad* mir3 = new Quad();
        mir3->setMaterial("reflectCoeff", 0.75f);
        mir3->updateTransformMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-4, 0, 4)), glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0, 1, 0)), glm::mat4(1.0f));
        mir3->setMaterial("all", glm::vec3(0.0f, 0.9f, 0.1f));
        objs.push_back(mir3);

        Quad* wall = new Quad();
        wall->updateTransformMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0,8)), glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0, 1, 0)), glm::mat4(1.0f));
        wall->setMaterial("all", glm::vec3(0.8f, 0.8f, 0.4f));
        objs.push_back(wall);
        
        PointLight* pointLight2 = new PointLight(glm::vec3(-2, 2, 4));
        pointLight2->setLightProp(glm::vec3(10, 10, 10), glm::vec3(100, 100, 100), glm::vec3(10, 10, 10));
        lights.push_back(pointLight2);
        ray = new TracingRay(nullptr, 0, dummyCameraPos, glm::vec3(-1, 0, 0));

        float distance;
        glm::vec3 normal;
        float tNear, tFar;


        //cout << is << " " << tNear << " " <<tFar << endl;
    }
    else if (scene == 2) {
        dummyCameraPos = glm::vec3(3, 0, 0);
        Sphere* sphere = new Sphere();
        sphere->setMaterial("reflectCoeff", 0.4f);
        sphere->setMaterial("refractCoeff", 0.4f);
        sphere->setMaterial("index", 1.9f);
        sphere->setMaterial("all", glm::vec3(0.0f, 0.5f, 0.2f));

        PointLight* pointLight = new PointLight(glm::vec3(3, 2, -5));
        lights.push_back(pointLight);
        ray = new TracingRay(nullptr, 0, dummyCameraPos, glm::vec3(-0.9, -0.3f, 0.1f));
        objs.push_back(sphere);
    }

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        step += deltaTime/2.0f;
        // input
        // -----
        processInput(window);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (PointLight* light : lights) {
            depthMaps.push_back(light->renderDepthMap());
           for (Object* obj : objs) {
               obj->drawDepth(light);
           }
        }
        //cout << Util::vec3ToString(camera.Position) << endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        for (unsigned int i = 0; i < lights.size(); i++) {
            glActiveTexture(GL_TEXTURE0+i);
            glBindTexture(GL_TEXTURE_CUBE_MAP, depthMaps[i]);
        }
        draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    deleteAll();
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void draw() {
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for (Object* obj : objs) {
        obj->drawPhong(&lights, camera.Position, projection, view);
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (PointLight* light : lights)
        light->draw(projection, view);
    /*sphere->draw(projection, view);*/
    ray->trace(objs, lights, dummyCameraPos, projection, view,step);
    //cout << Util::vec3ToString(camera.Position) << endl;
}

void deleteAll() {
   
  
   for (auto obj : objs)
        delete obj;
    objs.clear();
    
    for (auto light : lights)
        delete light;
    lights.clear();
    delete ray; 

}