#pragma once

#include <glm/glm.hpp>
using namespace glm;

class Camera {
public:
	vec3 front;
	vec3 up;
	vec3 right;
	vec3 dir;

	vec3 position;
	float fov;
	float zoom;
	mat4 rotation;
	Camera(vec3 position, float fov);

	void rotate(float x, float y, float z);

	mat4 getProjection();
	mat4 getView();
	void updateVectors();
};