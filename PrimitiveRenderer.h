#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
class PrimitiveRenderer
{
public:
    PrimitiveRenderer(float cPosX, float cPosY, float cPosZ);
    void setCPosX(float newCPosX);
    void setCPosY(float newCPosY);
    void setCPosZ(float newCPosZ);
    float getCPosX();
    float getCPosY();
    float getCPosZ();
    int getIndicesSize();
    unsigned int *getIndicesData();
    float *getVertexData();
    void draw();
    void cubeUp();
    void cubeDown();
    void cubeLeft();
    void cubeRight();
    void cubeForward();
    void cubeBack();
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
    void scaleUp(float factor);
    void scaleDown(float factor);
private:
    std::vector<float> vertex;
    std::vector<unsigned int> indices;
    float cubePosX;
    float cubePosY;
    float cubePosZ;

    float cubeRotX;
    float cubeRotY;
    float cubeRotZ;
    float cubeScale;
};