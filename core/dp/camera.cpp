//camera.cpp
#pragma once

#include "camera.h"

namespace dp {
	void CameraController::moveCamera(GLFWwindow* window, dp::Camera* camera, dp::CameraController* cameraCon, float deltaTime)
	{
		bool prevFirstMouse = cameraCon->firstMouse;

		if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
			//Release cursor
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			cameraCon->firstMouse = true;
			return;
		}
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		if (cameraCon->firstMouse) {
			cameraCon->firstMouse = false;
			cameraCon->prevMouseX = mouseX;
			cameraCon->prevMouseY = mouseY;
		}

		float mouseDeltaX = (float)(mouseX - cameraCon->prevMouseX);
		float mouseDeltaY = (float)(mouseY - cameraCon->prevMouseY);

		cameraCon->prevMouseX = mouseX;
		cameraCon->prevMouseY = mouseY;


		cameraCon->yaw += mouseDeltaX * cameraCon->mouseSensitivity;
		cameraCon->pitch -= mouseDeltaY * cameraCon->mouseSensitivity;
		cameraCon->pitch = ew::Clamp(cameraCon->pitch, -89.0f, 89.0f);

		float yawRad = ew::Radians(cameraCon->yaw);
		float pitchRad = ew::Radians(cameraCon->pitch);

		ew::Vec3 forward;
		forward.x = cosf(pitchRad) * sinf(yawRad);
		forward.y = sinf(pitchRad);
		forward.z = cosf(pitchRad) * -cosf(yawRad);
		forward = ew::Normalize(forward);

		ew::Vec3 right = ew::Normalize(ew::Cross(forward, ew::Vec3(0, 1, 0)));
		ew::Vec3 up = ew::Normalize(ew::Cross(right, forward));

		float speed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) ? cameraCon->sprintMoveSpeed : cameraCon->moveSpeed;
		float moveDelta = speed * deltaTime;

		if (glfwGetKey(window, GLFW_KEY_W))
			camera->pos += forward * moveDelta;

		if (glfwGetKey(window, GLFW_KEY_S))
			camera->pos -= forward * moveDelta;

		if (glfwGetKey(window, GLFW_KEY_D))
			camera->pos += right * moveDelta;

		if (glfwGetKey(window, GLFW_KEY_A))
			camera->pos -= right * moveDelta;

		if (glfwGetKey(window, GLFW_KEY_E))
			camera->pos += up * moveDelta;

		if (glfwGetKey(window, GLFW_KEY_Q))
			camera->pos -= up * moveDelta;

		camera->target = camera->pos + forward;
	}

	void CameraController::resetCamera(dp::Camera& camera, dp::CameraController& cameraController, float aspect)
	{
		camera.pos = ew::Vec3(0, 0, 3);
		camera.target = ew::Vec3(0);
		camera.aspect = aspect;
		camera.fov = 60.0f;
		camera.orthoSize = 6.0f;
		camera.near = 0.1f;
		camera.far = 100.0f;
		camera.ortho = false;

		cameraController.yaw = 0.0f;
		cameraController.pitch = 0.0f;
	}
}