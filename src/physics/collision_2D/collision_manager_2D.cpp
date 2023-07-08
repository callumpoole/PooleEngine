#include "poole/physics/collision_2D/collision_manager_2D.h"

#include "poole/physics/collision_2D/box_collider_2D.h"

#include "poole/rendering/renderer2D.h"

namespace Poole
{
	std::vector<std::shared_ptr<Collider2D>> ColliderManager2D::Colliders;
	std::vector<std::shared_ptr<Collider2D>> ColliderManager2D::CollidersToFreeAtEndOfTick;

	void ColliderManager2D::BeginScene()
	{
		for (size_t i = 0; i < Colliders.size(); i++)
		{
			for (size_t j = i + 1; j < Colliders.size(); j++)
			{
				const bool colliding = TestCollisionBase(*Colliders[i].get(), *Colliders[j].get());
				Colliders[i]->colliding = colliding;
				Colliders[j]->colliding = colliding;
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

	bool ColliderManager2D::TestCollisionBase(const Collider2D& a, const Collider2D& b)
	{
		const BoxCollider2D* b1 = dynamic_cast<const BoxCollider2D*>(&a);
		const BoxCollider2D* b2 = dynamic_cast<const BoxCollider2D*>(&b);
		if (b1 && b2)
		{
			return TestCollision(*b1, *b2);
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
				(a_c.UL - a_c.LL), //A's horizontal axis of projection
				(a_c.UL - a_c.UR), //A's vertical	axis of projection
				(b_c.UL - b_c.LL), //B's horizontal	axis of projection
				(b_c.UL - b_c.UR)  //B's vertical	axis of projection
			};

			auto proj_minmax = [](const BoxCollider2D::Corners& corners, fvec2 axis) {
				const f32 ul = glm::dot(corners.UL, axis) / glm::length2(axis);
				const f32 ur = glm::dot(corners.UR, axis) / glm::length2(axis);
				const f32 ll = glm::dot(corners.LL, axis) / glm::length2(axis);
				const f32 lr = glm::dot(corners.LR, axis) / glm::length2(axis);

				return std::pair{ std::min({ul,ur,ll,lr}), std::max({ul,ur,ll,lr}) };
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
}