#include "poole/physics/collision_2D/collision_manager_2D.h"

#include "poole/physics/collision_2D/box_collider_2D.h"

namespace Poole
{
	std::vector<Collider2D*> ColliderManager2D::Colliders;
	std::vector<Collider2D*> ColliderManager2D::CollidersToFreeAtEndOfTick;

	auto ColliderManager2D::MakeCollider() -> ColliderUniquePtr
	{
		BoxCollider2D* b = new BoxCollider2D;
		Colliders.push_back(b);
		return ColliderUniquePtr(b);
	}

	void ColliderManager2D::BeginScene()
	{
		for (size_t i = 0; i < Colliders.size(); i++)
		{
			for (size_t j = i + 1; j < Colliders.size(); j++)
			{
				const bool colliding = TestCollision(Colliders[i], Colliders[j]);
				Colliders[i]->colliding = colliding;
				Colliders[j]->colliding = colliding;
			}
		}
	}

	void ColliderManager2D::EndScene()
	{
		for (Collider2D* p : CollidersToFreeAtEndOfTick)
		{
			std::erase(Colliders, p);
			delete p;
		}
		CollidersToFreeAtEndOfTick.clear();
	}

	bool ColliderManager2D::TestCollision(Collider2D* a, Collider2D* b)
	{
		const BoxCollider2D* b1 = dynamic_cast<BoxCollider2D*>(a);
		const BoxCollider2D* b2 = dynamic_cast<BoxCollider2D*>(b);
		if (b1 && b2)
		{
			const fvec2 b1min = b1->GetMin();
			const fvec2 b1max = b1->GetMax();
			const fvec2 b2min = b2->GetMin();
			const fvec2 b2max = b2->GetMax();

			if ((b1min.x < b2max.x) && (b1max.x > b2min.x))
			{
				if ((b1min.y < b2max.y) && (b1max.y > b2min.y))
				{
					return true;
				}
			}
		}
		return false;
	}
}