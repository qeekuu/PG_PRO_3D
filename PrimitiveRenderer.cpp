#include "PrimitiveRenderer.h"
#include <iostream>

void PrimitiveRenderer::addVertex(float x, float y, float z)
{
    vertex.push_back(x);
    vertex.push_back(y);
    vertex.push_back(z);
}

void PrimitiveRenderer::addIndex(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}

float* PrimitiveRenderer::getData()
{
    return vertex.data();
}

unsigned int* PrimitiveRenderer::getIndexData()
{
    return indices.data();
}

size_t PrimitiveRenderer::getVertexCount()
{
    return vertex.size() / 3;
}

size_t PrimitiveRenderer::getIndexCount()
{
    return indices.size();
}