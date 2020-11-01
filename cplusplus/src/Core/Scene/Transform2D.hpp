#pragma once
#include "Core/Core.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

struct Transform2D {
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec2 origin;
	glm::vec3 rotation;
	glm::mat4 localToWorld;
	Transform2D(glm::vec2 position, glm::vec2 scale, glm::vec2 origin, glm::vec3 rotation) : position(glm::vec3(position.x, position.y, 0)), scale(glm::vec3(scale.x, scale.y, 1)), origin(origin), rotation(rotation) {
		rebuildMatrix();
	}

	void rebuildMatrix() {
		glm::mat4 t(1);
		glm::mat4 s(1);
		glm::mat4 r(1);
		
		t = glm::translate(t, position);
		r = glm::eulerAngleXYZ(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z));
		s = glm::scale(s, scale);
		
		localToWorld = t * r * s;
	}

	void setTRS(glm::mat4 trs) {
		position = glm::vec3(trs[3][0], trs[3][1], trs[3][2]);
		float scaleX = glm::length(glm::vec3(trs[0][0], trs[0][1], trs[0][2]));
		float scaleY = glm::length(glm::vec3(trs[1][0], trs[1][1], trs[1][2]));
		float scaleZ = glm::length(glm::vec3(trs[2][0], trs[2][1], trs[2][2]));
		scale = glm::vec3(scaleX, scaleY, scaleZ);
		rotation = eulerAngles(trs);
		localToWorld = trs;
	}

private:
	glm::vec3 eulerAngles(glm::mat4 trs) const {
		float scaleX = glm::length(glm::vec3(trs[0][0], trs[0][1], trs[0][2]));
		float scaleY = glm::length(glm::vec3(trs[1][0], trs[1][1], trs[1][2]));
		float scaleZ = glm::length(glm::vec3(trs[2][0], trs[2][1], trs[2][2]));
		auto rotationMatrix = glm::mat3(0);
		rotationMatrix[0] = trs[0] / scaleX;
		rotationMatrix[1] = trs[1] / scaleY;
		rotationMatrix[2] = trs[2] / scaleZ;
		glm::quat quaternion = glm::quat_cast(rotationMatrix);
		return glm::degrees(glm::eulerAngles(quaternion));
	}
};
