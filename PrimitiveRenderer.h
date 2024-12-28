#pragma once
#include <vector>

class PrimitiveRenderer
{
public:
    void addVertex(float x, float y, float z);
    void addIndex(unsigned int i1, unsigned int i2, unsigned int i3);
    float* getData();
    unsigned int* getIndexData();
    size_t getVertexCount();
    size_t getIndexCount();

private:
    std::vector<float> vertex;  
    std::vector<unsigned int> indices;  
};