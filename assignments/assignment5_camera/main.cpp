//main.cpp
#pragma once

#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <ew/procGen.h>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <dp/camera.h>
#include <dp/shader.h>
#include <dp/texture.h>
#include <dp/transform.h>
#include <dp/transformations.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void moveCamera(GLFWwindow* window, Camera* camera, float deltaTime);

//Projection will account for aspect ratio!
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const int NUM_CUBES = 4;
ew::Transform cubeTransforms[NUM_CUBES];
dp::Camera camera;
dp::CameraController cameraCon;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth testing - required for depth sorting!
	glEnable(GL_DEPTH_TEST);

	dp::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	
	//Cube mesh
	ew::Mesh cubeMesh(ew::createCube(0.5f));

	//Cube positions
	for (size_t i = 0; i < NUM_CUBES; i++)
	{
		cubeTransforms[i].position.x = i % (NUM_CUBES / 2) - 0.5;
		cubeTransforms[i].position.y = i / (NUM_CUBES / 2) - 0.5;
	}

	float prevtime;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float time = (float)glfwGetTime(); //Timestamp of current frame
		float deltaTime = time - prevTime;
		prevTime = time;

		moveCamera(window, &camera, deltaTime);
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set uniforms
		shader.use();
		shader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());

		//TODO: Set model matrix uniform
		for (size_t i = 0; i < NUM_CUBES; i++)
		{
			//Construct model matrix
			shader.setMat4("_Model", cubeTransforms[i].getModelMatrix());
			cubeMesh.draw();
		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Text("Cubes");
			for (size_t i = 0; i < NUM_CUBES; i++)
			{
				ImGui::PushID(i);
				if (ImGui::CollapsingHeader("Transform")) {
					ImGui::DragFloat3("Position", &cubeTransforms[i].position.x, 0.05f);
					ImGui::DragFloat3("Rotation", &cubeTransforms[i].rotation.x, 1.0f);
					ImGui::DragFloat3("Scale", &cubeTransforms[i].scale.x, 0.05f);
				}
				ImGui::PopID();
			}

			if (ImGui::CollapsingHeader("Camera")) {
				ImGui::DragFloat3("Position", &camera.pos.x, 0.1f);
				ImGui::DragFloat3("Target", &camera.target.x, 0.1f);
				ImGui::Checkbox("Orthographic", &camera.ortho);
				if (camera.ortho) {
					ImGui::DragFloat("Ortho Height", &camera.orthoSize, 0.1f);
				}
				else {
					ImGui::SliderFloat("FOV", &camera.fov, 0.0f, 180.0f);
				}
				ImGui::DragFloat("Near Plane", &camera.near, 0.1f, 0.0f);
				ImGui::DragFloat("Far Plane", &camera.far, 0.1f, 0.0f);
				ImGui::DragFloat("Move Speed", &cameraCon.moveSpeed, 0.1f);
				ImGui::DragFloat("Sprint Speed", &cameraCon.sprintMoveSpeed, 0.1f);
			}
			ImGui::End();
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void moveCamera(GLFWwindow* window, dp::Camera* camera, float deltaTime)
{
	bool prevFirstMouse = firstMouse;

	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
		//Release cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstMouse = true;
		return;
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	if (firstMouse) {
		firstMouse = false;
		prevMouseX = mouseX;
		prevMouseY = mouseY;
	}

	float mouseDeltaX = (float)(mouseX - prevMouseX);
	float mouseDeltaY = (float)(mouseY - prevMouseY);

	prevMouseX = mouseX;
	prevMouseY = mouseY;


	yaw += mouseDeltaX * mouseSensitivity;
	pitch -= mouseDeltaY * mouseSensitivity;
	pitch = ew::Clamp(pitch, -89.0f, 89.0f);

	float yawRad = ew::Radians(yaw);
	float pitchRad = ew::Radians(pitch);

	ew::Vec3 forward;
	forward.x = cosf(pitchRad) * sinf(yawRad);
	forward.y = sinf(pitchRad);
	forward.z = cosf(pitchRad) * -cosf(yawRad);
	forward = ew::Normalize(forward);

	ew::Vec3 right = ew::Normalize(ew::Cross(forward, ew::Vec3(0, 1, 0)));
	ew::Vec3 up = ew::Normalize(ew::Cross(right, forward));

	float speed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) ? sprintMoveSpeed : moveSpeed;
	float moveDelta = speed * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W))
		camera->position += forward * moveDelta;

	if (glfwGetKey(window, GLFW_KEY_S))
		camera->position -= forward * moveDelta;

	if (glfwGetKey(window, GLFW_KEY_D))
		camera->position += right * moveDelta;

	if (glfwGetKey(window, GLFW_KEY_A))
		camera->position -= right * moveDelta;

	if (glfwGetKey(window, GLFW_KEY_E))
		camera->position += up * moveDelta;

	if (glfwGetKey(window, GLFW_KEY_Q))
		camera->position -= up * moveDelta;

	camera->target = camera->position + forward;
}