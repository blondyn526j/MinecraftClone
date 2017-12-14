#pragma once

#define SPEED 0.13f

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "globals.h"
#include "input.h"
#include "blocks.h"
//#include "chunkManager.h"

class Camera
{
public:
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 position;
	glm::vec3 left;
	glm::vec3 oldForward;
	//ChunkManager *chunkManager;

	Camera(const glm::vec3 &pos, float fov, float aspect, float zNear, float zFar)
	{
		m_perspective = glm::perspective(fov, aspect, zNear, zFar);
		position = pos;
		forward = glm::vec3(0, 0, 1);
		up = glm::vec3(0, -1, 0);
		//this->chunkManager = chunkManager;
	}
	~Camera()
	{

	}

	inline glm::mat4 GetViewProjectoin() const
	{
		return m_perspective * glm::lookAt(position, position + forward, up);
	}

	void Update(Input *input)
	{
		RotateBy(input->xMouse / 100.0f, input->yMouse / 100.0f);

		left = glm::vec3(forward.z, 0, -forward.x);
		left = glm::normalize(left);

		glm::vec3 forward2D = glm::normalize(glm::vec3(forward.x, 0, forward.z));

		if (noclip)
		{
			position += forward * 3.0f * SPEED * (float)input->vertical + left * 3.0f * SPEED * (float)input->horizontal;
			return;
		}
		m_velocity = glm::vec3(0, m_velocity.y, 0) + forward2D * SPEED * (float)input->vertical + left * SPEED * (float)input->horizontal;

		bool isGrounded = chunkManager.m_xyzToBlock(position.x, position.y + m_velocity.y - 2, position.z) != Blocks::BLOCK_AIR;
		if(!isGrounded)
		{
			m_velocity.y += -0.01;
		}
		else
		{
			m_velocity.y = 0;
			if(input->jump)
			m_velocity.y = 0.19;
		}

		//position += forward * SPEED * vertical + left * SPEED * horizontal;
		position += m_velocity;
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
	glm::vec3 m_velocity = glm::vec3(0, 0.05, 0);
	glm::mat4 m_perspective;
	float m_angle = 0;
};

