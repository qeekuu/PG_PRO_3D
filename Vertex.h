#pragma once
class Vertex
{
public:
	Vertex(float x, float y, float z) : x{ x }, y{ y }, z{ z }
	{
	}
	float getCoordinatesX()
	{
		return x;
	}
	float getCoordinatesY()
	{
		return y;
	}
	float getCoordinatesZ()
	{
		return z;
	}
private:
	float x, y, z;
};

