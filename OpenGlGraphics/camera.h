#pragma once

#define SPEED 0.65f

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera
{
public:
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 position;
	glm::vec3 left;
	glm::vec3 oldForward;

	Camera(const glm::vec3 &pos, float fov, float aspect, float zNear, float zFar)
	{
		m_perspective = glm::perspective(fov, aspect, zNear, zFar);
		position = pos;
		forward = glm::vec3(0, 0, 1);
		up = glm::vec3(0, -1, 0);
	}
	~Camera()
	{

	}

	inline glm::mat4 GetViewProjectoin() const
	{
		return m_perspective * glm::lookAt(position, position + forward, up);
	}

	void Update(float vertical, float horizontal, float xMouse, float yMouse)
	{
		left = glm::vec3(forward.z, 0, -forward.x);
		left /= glm::length(left);
		position += forward * SPEED * vertical + left * SPEED * horizontal;
		RotateBy(xMouse / 100.0f, yMouse / 100.0f);
	}

	void RotateBy(float xAngle, float yAngle)
	{
		oldForward = forward;
		forward = glm::vec4(forward, 1.0f) * glm::rotate(-xAngle, up) * glm::rotate(yAngle, glm::cross(up, forward));

		if (oldForward.x * forward.x <= 0.001f  && oldForward.z * forward.z <= 0.001f)
		{
			forward = glm::vec4(oldForward, 1.0f) * glm::rotate(-xAngle, up);
		}
		//up = glm::vec4(up, 1.0f) * glm::rotate(yAngle, glm::cross(up, oldForward));
		//glm::quat
	}

private:
	glm::mat4 m_perspective;
	float m_angle = 0;
};

