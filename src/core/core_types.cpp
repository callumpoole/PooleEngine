#include "core.h" //TODO: Should be "core/core_types.h" but wasn't working. 

#include "glm/gtc/matrix_transform.hpp"

fmat4 ftransform2D::MakeTransformMatrix() const
{
	const fmat4 shearMat = {
				  1, shear.y, 0, 0,
			shear.x,	   1, 0, 0,
				  0,	   0, 1, 0,
				  0,	   0, 0, 1,
	};

	//TODO: Optimise, put one func inside the next instead of the identity fmat4
	return
		glm::translate(fmat4(1.0f), position) *
		glm::rotate(fmat4(1.0f), rotation, fvec3{ 0,0,1 }) *
		shearMat *
		glm::scale(fmat4(1.0f), fvec3{ scale.x, scale.y, 1.f });
}
