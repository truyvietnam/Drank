#pragma once
#include <math.h>

struct vec3 {
	float x;
	float y;
	float z;

	vec3 operator-(vec3 b) {
		return { x - b.x, y - b.y, z - b.z };
	}

	vec3 operator+(vec3 b) {
		return { x + b.x, y + b.y, z + b.z };
	}
};

struct vec2 {
	float x;
	float y;
};

vec3 GetForwardVector(vec2 rot) {
	vec3 forward{};

	forward.x = sin(rot.x) * cos(rot.y);
	forward.y = sin(rot.y);
	forward.z = cos(rot.x) * cos(rot.y);

	return forward;
}

vec2 CalcAngle(vec3 src, vec3 target) {
	vec3 delta = target - src;
	float dist = sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);

	vec2 angles{};

	angles.x = atan2f(delta.x, delta.z);
	angles.y = asin(delta.y / dist);

	return angles;
}

