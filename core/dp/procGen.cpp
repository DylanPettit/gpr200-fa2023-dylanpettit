//procGen.cpp

#include "procGen.h"

namespace dp {
	ew::MeshData createPlane(float width, float height, int subdivisions)
	{
		for (row = 0; row <= subdivisions; row++)
			for (col = 0; col <= subdivisions; col++)
			{
				v.x = width * (col / subdivisions);
				v.z = -height * (row / subdivisions);
				vertices.push_back(v);
			}

		columns = subdivisions + 1;
		for (row = 0; row < subdivisions; row++)
			for (col = 0; col < subdivisions; col++)
			{
				start = row * columns + col;
				//Bottom right triangle
				indices.push_back(start);
				indices.push_back(start + 1);
				indices.push_back(start + columns + 1);
				//Top right triangle
				indices.push_back(start);
				indices.push_back(start + columns);
				indices.push_back(start + columns + 1);
			}
	}

	ew::MeshData createCylinder(float height, float radius, int numSegments)
	{
		float topY = height / 2.0f;
		float bottomY = -topY;
		vertices.push_back({ 0,topY,0 });
		float thetaStep = 2PI / numSegments;
			for (i = 0; i <= numSegments)
			{
				float theta = i * thetaStep;
				v.x = cos(theta) * radius;
				v.z = sin(theta) * radius;
				v.y = topY;
				vertices.push_back(v);
			}

			for (i = 0; i <= numSegments)
			{
				float theta = i * thetaStep;
				v.x = cos(theta) * radius;
				v.z = sin(theta) * radius;
				v.y = bottomY;
				vertices.push_back(v);
			}

			vertices.push_back({ 0,bottomY,0 });
	}

	ew::MeshData createSphere(float radius, int numSegments)
	{

	}
}