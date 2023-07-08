#include "poole/physics/collision_2D/collision_manager_2D.h"

#include "poole/physics/collision_2D/box_collider_2D.h"
#include "poole/physics/collision_2D/circle_collider_2D.h"

#include "poole/rendering/renderer2D.h"

namespace Poole
{
	std::vector<std::shared_ptr<Collider2D>> ColliderManager2D::Colliders;
	std::vector<std::shared_ptr<Collider2D>> ColliderManager2D::CollidersToFreeAtEndOfTick;

	void ColliderManager2D::BeginScene()
	{
		//Reset from previous time
		for (size_t i = 0; i < Colliders.size(); i++)
		{
			Colliders[i]->m_Colliding = false;
		}
		for (size_t i = 0; i < Colliders.size(); i++)
		{
			for (size_t j = i + 1; j < Colliders.size(); j++)
			{
				//Maybe need to remove in future if we instead keep a list of colliders
				if (!Colliders[i]->m_Colliding && !Colliders[j]->m_Colliding)
				{
					const bool colliding = TestCollision(*Colliders[i].get(), *Colliders[j].get());
					Colliders[i]->m_Colliding |= colliding;
					Colliders[j]->m_Colliding |= colliding;
				}
			}
		}
	}

	void ColliderManager2D::EndScene()
	{
		for (std::shared_ptr<Collider2D> p : CollidersToFreeAtEndOfTick)
		{
			std::erase(Colliders, p);
		}
		CollidersToFreeAtEndOfTick.clear();
	}

	bool ColliderManager2D::TestCollision(const Collider2D& a, const Collider2D& b)
	{
		const BoxCollider2D* b1 = dynamic_cast<const BoxCollider2D*>(&a);
		const BoxCollider2D* b2 = dynamic_cast<const BoxCollider2D*>(&b);
		if (b1 && b2)
		{
			return TestCollision(*b1, *b2); //Box Box
		}
		const CircleCollider2D* c1 = dynamic_cast<const CircleCollider2D*>(&a);
		const CircleCollider2D* c2 = dynamic_cast<const CircleCollider2D*>(&b);
		if (c1 && c2)
		{
			return TestCollision(*c1, *c2); //Circle Circle
		}
		if (b1 && c2)
		{
			return TestCollision(*b1, *c2); //Box Circle
		}
		if (c1 && b2)
		{
			return TestCollision(*b2, *c1); //Box Circle (inputs swapped)
		}
		return false;
	}

	bool ColliderManager2D::TestCollision(const BoxCollider2D& a, const BoxCollider2D& b)
	{
		if (a.GetRadians() == 0 && b.GetRadians() == 0) //AABB
		{
			auto [amin, amax] = a.GetMinMaxUnrotated();
			auto [bmin, bmax] = b.GetMinMaxUnrotated();
			
			if ((amin.x < bmax.x) && (amax.x > bmin.x))
			{
				if ((amin.y < bmax.y) && (amax.y > bmin.y))
				{
					return true;
				}
			}
			return false;
		}
		else //SAT
		{
			const BoxCollider2D::Corners& a_c = a.GetCorners();
			const BoxCollider2D::Corners& b_c = b.GetCorners();

			const std::array<fvec2, 4> axes = { 
				(a_c.TL - a_c.BL), //A's horizontal axis of projection
				(a_c.TL - a_c.TR), //A's vertical	axis of projection
				(b_c.TL - b_c.BL), //B's horizontal	axis of projection
				(b_c.TL - b_c.TR)  //B's vertical	axis of projection
			};

			auto proj_minmax = [](const BoxCollider2D::Corners& corners, fvec2 axis) {
				const f32 tl = glm::dot(corners.TL, axis) / glm::length2(axis);
				const f32 tr = glm::dot(corners.TR, axis) / glm::length2(axis);
				const f32 bl = glm::dot(corners.BL, axis) / glm::length2(axis);
				const f32 br = glm::dot(corners.BR, axis) / glm::length2(axis);

				return std::pair{ std::min({tl,tr,bl,br}), std::max({tl,tr,bl,br}) };
			};

			for (fvec2 axis : axes)
			{
				auto [amin, amax] = proj_minmax(a_c, axis);
				auto [bmin, bmax] = proj_minmax(b_c, axis);

				if ((amin > bmax) || (amax < bmin))
				{
					return false;
				}
			}
			return true;

			//One resource: https://www.gamedev.net/tutorials/_/technical/game-programming/2d-rotated-rectangle-collision-r2604/
			//Better resource: https://gamedevelopment.tutsplus.com/collision-detection-using-the-separating-axis-theorem--gamedev-169t
		}
	}
	
	bool ColliderManager2D::TestCollision(const CircleCollider2D& a, const CircleCollider2D& b)
	{
		return glm::length2(a.GetPosition() - b.GetPosition()) < square(a.GetRadius() + b.GetRadius());
	}

	bool ColliderManager2D::TestCollision(const BoxCollider2D& a, const CircleCollider2D& b)
	{
		//TODO: see if there's a cheaper one for if the box isn't rotated (AABB)

		const BoxCollider2D::Corners& a_c = a.GetCorners();
		const std::array<fvec2, 3> axes = {
			(a_c.TL - a_c.BL), //A's horizontal axis of projection
			(a_c.TL - a_c.TR), //A's vertical	axis of projection
			(a.GetPosition() - b.GetPosition()), //The line from circle to square axis of projection
		};
		auto proj_minmax_rect = [](const BoxCollider2D::Corners& corners, fvec2 axis) {
			const f32 tl = glm::dot(corners.TL, axis) / glm::length2(axis);
			const f32 tr = glm::dot(corners.TR, axis) / glm::length2(axis);
			const f32 bl = glm::dot(corners.BL, axis) / glm::length2(axis);
			const f32 br = glm::dot(corners.BR, axis) / glm::length2(axis);

			return std::pair{ std::min({tl,tr,bl,br}), std::max({tl,tr,bl,br}) };
		};
		auto proj_minmax_circle = [](const CircleCollider2D& circle, fvec2 axis) {
			const fvec2 axis_norm = glm::normalize(axis);
			const f32 min = glm::dot(circle.GetPosition() - circle.GetRadius() * axis_norm, axis) / glm::length2(axis);
			const f32 max = glm::dot(circle.GetPosition() + circle.GetRadius() * axis_norm, axis) / glm::length2(axis);
			
			auto out = std::pair{ min, max };

			if (out.first > out.second)
				swap(out.first, out.second);

			return out;
		};
		for (fvec2 axis : axes)
		{
			auto [amin, amax] = proj_minmax_rect(a_c, axis);
			auto [bmin, bmax] = proj_minmax_circle(b, axis);

			if ((amin > bmax) || (amax < bmin))
			{
				return false;
			}
		}
		return true;
	}
}