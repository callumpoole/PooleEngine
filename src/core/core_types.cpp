#include "core.h" //TODO: Should be "core/core_types.h" but wasn't working. 

#include "glm/gtc/matrix_transform.hpp"

fmat4 ftransform2D::MakeTransformMatrix() const
{
	if (shear == fvec2(0)) [[likely]]
	{
		return 
			glm::scale(
				glm::rotate(
					glm::translate(fmat4(1.0f), position)
				, rotation, fvec3{ 0,0,1 })
			, fvec3{ scale.x, scale.y, 1.f });

		//Same as: 
		//return glm::translate(fmat4(1.0f), position) * glm::rotate(fmat4(1.0f), rotation, fvec3{ 0,0,1 }) * glm::scale(fmat4(1.0f), fvec3{ scale.x, scale.y, 1.f });
	}
	else [[unlikely]]
	{
		auto shear2D = [](const fmat4& m, fvec2 const& v) -> fmat4
		{
			const fmat4 shearMat = {
				  1, v.y, 0, 0,
				v.x,   1, 0, 0,
				  0,   0, 1, 0,
				  0,   0, 0, 1,
			};
			return m * shearMat;
		};

		return
			glm::scale(
				shear2D(
					glm::rotate(
						glm::translate(fmat4(1.0f), position)
						, rotation, fvec3{ 0,0,1 })
					, shear)
				, fvec3{ scale.x, scale.y, 1.f });
	}
}

fmat4 ftransform2D::MakeRotationMatrix() const
{
	return glm::rotate(fmat4(1.0f), rotation, fvec3{ 0,0,1 });
}
