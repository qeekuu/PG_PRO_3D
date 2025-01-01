#include "PrimitiveRenderer.h"
#include <iostream>
PrimitiveRenderer::PrimitiveRenderer(float cPosX, float cPosY, float cPosZ) : cubePosX{cPosX},cubePosY{cPosY},cubePosZ{cPosZ},
cubeRotX{0.f}, cubeRotY{0.f}, cubeRotZ{0.f},cubeScale{1.0f}
{
    //Wierzcholki

    //Przednia sciana
    vertex.push_back(1.0f);  vertex.push_back(1.0f);  vertex.push_back(1.0f);
    vertex.push_back(-1.0f); vertex.push_back(1.0f);  vertex.push_back(1.0f);
    vertex.push_back(-1.0f); vertex.push_back(-1.0f); vertex.push_back(1.0f);
    vertex.push_back(1.0f);  vertex.push_back(-1.0f); vertex.push_back(1.0f);

    //Tylna sciana
    vertex.push_back(1.0f);  vertex.push_back(1.0f);  vertex.push_back(-1.0f);
    vertex.push_back(-1.0f); vertex.push_back(1.0f);  vertex.push_back(-1.0f);
    vertex.push_back(-1.0f); vertex.push_back(-1.0f); vertex.push_back(-1.0f);
    vertex.push_back(1.0f);  vertex.push_back(-1.0f); vertex.push_back(-1.0f);

    //Prawa sciana
    vertex.push_back(1.0f);  vertex.push_back(1.0f);  vertex.push_back(1.0f);
    vertex.push_back(1.0f);  vertex.push_back(-1.0f); vertex.push_back(1.0f);
    vertex.push_back(1.0f);  vertex.push_back(-1.0f); vertex.push_back(-1.0f);
    vertex.push_back(1.0f);  vertex.push_back(1.0f);  vertex.push_back(-1.0f);

    //Lewa sciana
    vertex.push_back(-1.0f); vertex.push_back(1.0f);  vertex.push_back(1.0f);
    vertex.push_back(-1.0f); vertex.push_back(-1.0f); vertex.push_back(1.0f);
    vertex.push_back(-1.0f); vertex.push_back(-1.0f); vertex.push_back(-1.0f);
    vertex.push_back(-1.0f); vertex.push_back(1.0f);  vertex.push_back(-1.0f);

    //Gorna sciana
    vertex.push_back(1.0f);  vertex.push_back(1.0f);  vertex.push_back(1.0f);
    vertex.push_back(-1.0f); vertex.push_back(1.0f);  vertex.push_back(1.0f);
    vertex.push_back(-1.0f); vertex.push_back(1.0f);  vertex.push_back(-1.0f);
    vertex.push_back(1.0f);  vertex.push_back(1.0f);  vertex.push_back(-1.0f);

    //Dolna sciana
    vertex.push_back(1.0f);  vertex.push_back(-1.0f); vertex.push_back(1.0f);
    vertex.push_back(-1.0f); vertex.push_back(-1.0f); vertex.push_back(1.0f);
    vertex.push_back(-1.0f); vertex.push_back(-1.0f); vertex.push_back(-1.0f);
    vertex.push_back(1.0f);  vertex.push_back(-1.0f); vertex.push_back(-1.0f);

    //Indeksy

    //Przednia
    indices.push_back(0);  indices.push_back(1);  indices.push_back(2);
    indices.push_back(0);  indices.push_back(2);  indices.push_back(3);

    //Tylna
    indices.push_back(4);  indices.push_back(5);  indices.push_back(6);
    indices.push_back(4);  indices.push_back(6);  indices.push_back(7);

    //Prawa
    indices.push_back(8);  indices.push_back(9);  indices.push_back(10);
    indices.push_back(8);  indices.push_back(10); indices.push_back(11);

    //Lewa
    indices.push_back(12); indices.push_back(13); indices.push_back(14);
    indices.push_back(12); indices.push_back(14); indices.push_back(15);

    //Gorna
    indices.push_back(16); indices.push_back(17); indices.push_back(18);
    indices.push_back(16); indices.push_back(18); indices.push_back(19);

    //Dolna
    indices.push_back(20); indices.push_back(21); indices.push_back(22);
    indices.push_back(20); indices.push_back(22); indices.push_back(23);
    
    //Ladowanie tablic
    glEnableClientState(GL_VERTEX_ARRAY);
    //Ustawienie wskaznika na tablice
    glVertexPointer(3, GL_FLOAT, 0, vertex.data());
}

float PrimitiveRenderer::getCPosX()
{
    return this->cubePosX;
}

float PrimitiveRenderer::getCPosY()
{
    return this->cubePosY;
}

float PrimitiveRenderer::getCPosZ()
{
    return this->cubePosZ;
}

void PrimitiveRenderer::setCPosX(float newCPosX)
{
    this->cubePosX = newCPosX;
}
void PrimitiveRenderer::setCPosY(float newCPosY)
{
    this->cubePosY = newCPosY;
}
void PrimitiveRenderer::setCPosZ(float newCPosZ)
{
    this->cubePosZ = newCPosZ;
}
int PrimitiveRenderer::getIndicesSize()
{
    return this->indices.size();
}
unsigned int *PrimitiveRenderer::getIndicesData()
{
    return this->indices.data();
}
float *PrimitiveRenderer::getVertexData()
{
    return this->vertex.data();
}
void PrimitiveRenderer::draw()
{
    //Przesuniecie
    glTranslatef(cubePosX, cubePosY, cubePosZ);

    //Rotacja
    glRotatef(cubeRotX, 1.0f, 0.0f, 0.0f);
    glRotatef(cubeRotY, 0.0f, 1.0f, 0.0f);
    glRotatef(cubeRotZ, 0.0f, 0.0f, 1.0f);

    //Skalowanie
    glScalef(cubeScale, cubeScale, cubeScale);

    //Kolor
    glColor3f(1.0f, 0.0f, 0.0f);

    //Rysuj 
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
}

void PrimitiveRenderer::cubeUp()
{
    this->cubePosY += 0.1f;
}

void PrimitiveRenderer::cubeDown()
{
    this->cubePosY -= 0.1f;
}

void PrimitiveRenderer::cubeLeft()
{
    this->cubePosX -= 0.1f;
}

void PrimitiveRenderer::cubeRight()
{
    this->cubePosX += 0.1f;
}

void PrimitiveRenderer::cubeForward()
{
    this->cubePosZ -= 0.1f;
}

void PrimitiveRenderer::cubeBack()
{
    this->cubePosZ += 0.1f;
}

void PrimitiveRenderer::rotateX(float angle)
{
    cubeRotX += angle;
}

void PrimitiveRenderer::rotateY(float angle)
{
    cubeRotY += angle;
}

void PrimitiveRenderer::rotateZ(float angle)
{
    cubeRotZ += angle;
}

void PrimitiveRenderer::scaleUp(float factor)
{
    cubeScale += factor;
}

void PrimitiveRenderer::scaleDown(float factor)
{
    cubeScale -= factor;
}