#pragma once
#include "../ew/ewMath/ewMath.h"
#include "transformations.h"
namespace ew {
	struct Transform {
		ew::Vec3 position = ew::Vec3(0.0f, 0.0f, 0.0f);
		ew::Vec3 rotation = ew::Vec3(0.0f, 0.0f, 0.0f); //Euler angles (Degrees)
		ew::Vec3 scale = ew::Vec3(1.0f, 1.0f, 1.0f);

		ew::Mat4 getModelMatrix() const {
			return dp::Translate(position)
				* dp::RotateY(ew::Radians(rotation.y))
				* dp::RotateX(ew::Radians(rotation.x))
				* dp::RotateZ(ew::Radians(rotation.z))
				* dp::Scale(scale);
		}
	};
}