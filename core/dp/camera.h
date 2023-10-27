//camera.h
#pragma once

#include "../ew/ewMath/ewMath.h"
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "../ew/external/glad.h"
#include "transformations.h"

namespace dp {
	struct Camera {
		ew::Vec3 pos; //Camera body position
		ew::Vec3 target; //Position to look at
		float fov; //Vertical field of view in degrees
		float aspect; //Screen width / Screen height
		float near; //Near plane distance (+Z)
		float far; //Far plane distance (+Z)
		bool ortho; //Perspective or orthographic?
		float orthoSize; //Height of orthographic frustum
		ew::Mat4 ViewMatrix()const { return dp::LookAt(pos, target, ew::Vec3(0, 1, 0)); } //World->View
		ew::Mat4 ProjectionMatrix()const //View->Clip
		{
			if (ortho)
				return dp::Orthographic(orthoSize, aspect, near, far);
			else
				return dp::Perspective(ew::Radians(fov), aspect, near, far);
		}
	};

	struct CameraController {
		float moveSpeed = 3.0f; //Default speed
		float sprintMoveSpeed = 6.0f; //Speed when left shift is held
		float mouseSensitivity = 0.1f; //mouse delta to degrees. Higher values = faster
		float yaw = 0.0f; //Degrees
		float pitch = 0.0f; //Degrees
		double prevMouseX = 0; //Stores previous mouse X position each frame
		double prevMouseY = 0; //Stores previous mouse Y position each frame
		bool firstMouse = true; //Used to get first frame mouse position
		};
}