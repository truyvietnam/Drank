#pragma once
#include <math.h>

struct vec3 {
	float x;
	float y;
	float z;

	vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

	vec3 operator-(vec3 b) {
		return { x - b.x, y - b.y, z - b.z };
	}

	vec3 operator+(vec3 b) {
		return { x + b.x, y + b.y, z + b.z };
	}

	bool isNotZero() {
		return 
			floor(x) != 0.0 &&
			floor(y) != 0.0 &&
			floor(z) != 0.0;
	}

	float length() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	vec3 normalized() const {
		float len = length();
		if (len == 0.f) return vec3();
		return vec3(x / len, y / len, z / len);
	}
};

struct vec2 {
	float x;
	float y;
};

struct vec4 {
	float x;
	float y;
	float z;
	float w;
};

struct matrix {
	float v[16];

	float operator[](int i) const { return v[i]; }
	float& operator[](int i) { return v[i]; }

	bool WorldToScreen(vec3 pos, vec2& playerscreen, float width, float height) {
		//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
		vec4 clipCoords{};

		clipCoords.x = pos.x * v[0] + pos.y * v[4] + pos.z * v[8] + v[12];
		clipCoords.y = pos.x * v[1] + pos.y * v[5] + pos.z * v[9] + v[13];
		clipCoords.z = pos.x * v[2] + pos.y * v[6] + pos.z * v[10] + v[14];
		clipCoords.w = pos.x * v[3] + pos.y * v[7] + pos.z * v[11] + v[15];

		if (clipCoords.w < 0.1f)
		{
			return false;
		}

		//perspective division, dividing by clip.W = Normalized Device Coordinates
		vec3 NDC{};
		NDC.x = clipCoords.x / clipCoords.w;
		NDC.y = clipCoords.y / clipCoords.w;
		NDC.z = clipCoords.z / clipCoords.w;

		//viewport tranform to screenCooords

		playerscreen.x = (width / 2.f * NDC.x) + (NDC.x + width / 2.f);
		playerscreen.y = -(height / 2.f * NDC.y) + (NDC.y + height / 2.f);

		return true;
	}
};

namespace Math {
	vec3 GetForwardVector(vec2 rot) {
		vec3 forward{};

		forward.x = -sin(rot.x) * cos(rot.y);
		forward.y = -sin(rot.y);
		forward.z = -cos(rot.x) * cos(rot.y);

		return forward;
	}

	vec3 GetForwardVector(vec3 pos1, vec3 pos2) {
		auto direction = pos2 - pos1;
		return direction.normalized();
	}

	vec2 CalcAngle(vec3 src, vec3 target) {
		vec3 delta = src - target;
		float dist = sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);

		vec2 angles{};

		angles.x = atan2f(delta.x, delta.z);
		angles.y = asin(delta.y / dist);

		return angles;
	}
}

