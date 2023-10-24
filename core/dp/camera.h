//camera.h
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
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
}
