#include "Math2.h"
#include "cmath"
#include "Matrix4.h"
#include "math.h"
#include "stdlib.h"

void rollRotation(XMFLOAT3 *vector, XMFLOAT3 rotation)
{
	XMFLOAT3 vec = *vector;

	Matrix4 matRotX, matRotY, matRotZ;
	matRotX = rotationX(rotation.x);
	matRotY = rotationY(rotation.y);
	matRotZ = rotationZ(rotation.z);

	matRotZ *= matRotX;
	matRotZ *= matRotY;

	vector->x = vec.x * matRotZ.m[0][0] + vec.y * matRotZ.m[1][0] + vec.z * matRotZ.m[2][0];
	vector->y = vec.x * matRotZ.m[0][1] + vec.y * matRotZ.m[1][1] + vec.z * matRotZ.m[2][1];
	vector->z = vec.x * matRotZ.m[0][2] + vec.y * matRotZ.m[1][2] + vec.z * matRotZ.m[2][2];
}

float vector3Length(XMFLOAT3 vector_)
{
	float x2 = vector_.x * vector_.x;
	float y2 = vector_.y * vector_.y;
	float z2 = vector_.z * vector_.z;
	return sqrt(x2 + y2 + z2);
}

XMFLOAT3 vector3Normalize(XMFLOAT3 vector_)
{
	XMFLOAT3 v;
	v.x = vector_.x / vector3Length(vector_);
	v.y = vector_.y / vector3Length(vector_);
	v.z = vector_.z / vector3Length(vector_);

	return v;
}

int RNG(int min, int max, bool preciseMode)
{
	if (!preciseMode) {
		return (rand() % (max + 1 - min) + min);
	}

	int ret = 0;
	do {
		ret = rand();
	} while (ret >= RAND_MAX - RAND_MAX % (max + 1 - min));
	ret = ret % (max + 1 - min) + min;
}
