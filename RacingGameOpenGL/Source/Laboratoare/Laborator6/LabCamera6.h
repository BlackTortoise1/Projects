#pragma once
#include <include/glm.h>
#include <include/math.h>
#include <Core/Engine.h>
#include <Component/SimpleScene.h>

namespace LabCamera6
{
	class Camera
	{
	public:
		Camera()
		{

		}

		Camera(WindowObject* window)
		{
			projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

			Set(Camera3rdOffset, glm::vec3(0), glm::vec3(0, 1, 0));
		}

		Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up, WindowObject* window)
		{
			Set(position, center, up);
		}

		~Camera()
		{ }


		void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
		{
			this->position = position;
			forward = glm::normalize(center - position);
			right = glm::cross(forward, up);
			this->up = glm::cross(right, forward);
		}

		void Update(glm::vec3 PlayerPosition, glm::vec3 PlayerRotations)
		{
			if(Camera3rd)
				position = PlayerPosition + Camera3rdOffset;
			
			if (Camera1st)
			{
				position = PlayerPosition + Camera1stOffset;
				RotateFirstPerson_OY(-CurrentRotations.y);
				CurrentRotations = PlayerRotations;
				RotateFirstPerson_OY(CurrentRotations.y);
			}
		}

		void ManageInput(int key) 
		{
			if (key == GLFW_KEY_C)
			{
				if (Camera3rd)
				{
					Camera3rd = false;
					Camera1st = true;

					RotateFirstPerson_OY(CurrentRotations.y);
				}
				else
				{
					if (Camera1st)
					{
						Camera3rd = true;
						Camera1st = false;

						RotateFirstPerson_OY(-CurrentRotations.y);
					}
				}
			}
		}

		void MoveForward(float distance)
		{
			glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
		}

		void TranslateForward(float distance)
		{
			glm::vec3 dir = glm::normalize(forward);
			position += dir*distance;
		}

		void TranslateUpword(float distance)
		{			
			glm::vec3 dir = glm::normalize(up);
			position += dir*distance;
		}

		void TranslateRight(float distance)
		{
			glm::vec3 dir = glm::normalize(right);
			position += dir*distance;
		}

		void RotateFirstPerson_OX(float angle)
		{
			glm::vec3 nForward = glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(forward, 1);
			forward = glm::normalize(glm::vec3(nForward));
			up = glm::cross(right, forward);
		}

		void RotateFirstPerson_OY(float angle)
		{
			glm::vec3 nForward = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1);
			glm::vec3 nRight = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1);
			right = glm::normalize(glm::vec3(nRight));
			forward = glm::normalize(glm::vec3(nForward));
			up = glm::cross(right, forward);
		}

		void RotateFirstPerson_OZ(float angle)
		{
			glm::vec3 nForward = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(forward)) * glm::vec4(right, 1);
			forward = glm::normalize(glm::vec3(nForward));
			up = glm::cross(right, forward);
		}

		glm::mat4 GetViewMatrix()
		{
			return glm::lookAt(position, position + forward, up);
		}

		glm::mat4 GetProjectionMatrix()
		{
			return projectionMatrix;
		}

	public:
		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;

		glm::mat4 projectionMatrix;
		glm::vec3 Camera3rdOffset = glm::vec3(0, 2.2, -4.5);
		glm::vec3 Camera1stOffset = glm::vec3(0, 0.2, 0.8);

		bool Camera3rd = true;
		bool Camera1st = false;

		glm::vec3 CurrentRotations;
	};
}