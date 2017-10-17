#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera
{
public:
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 position;

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

	void RotateBy(float yAngle, float xAngle)
	{
		glm::vec3 oldForward = forward;
		forward = glm::vec4(forward, 1.0f) * glm::rotate(-yAngle, up) * glm::rotate(xAngle, glm::cross(up, forward));

		//up = glm::vec4(up, 1.0f) * glm::rotate(yAngle, glm::cross(up, oldForward));
		//glm::quat
	}

private:
	glm::mat4 m_perspective;
	float m_angle = 0;
};

