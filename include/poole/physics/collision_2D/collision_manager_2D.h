#pragma once

#include "poole/core.h"
#include "collider_2D.h"

namespace Poole
{
	class ColliderManager2D
	{
	public:
		static std::vector<Collider2D*> Colliders;
		static std::vector<Collider2D*> CollidersToFreeAtEndOfTick;


		struct Deleter
		{
			void operator()(Collider2D* p)
			{
				CollidersToFreeAtEndOfTick.push_back(p);
			}
		};
		//using ColliderUniquePtr = std::unique_ptr<Collider2D, decltype([](Collider2D* p){ CollidersToFreeAtEndOfTick.push_back(p); })>;
		using ColliderUniquePtr = std::unique_ptr<Collider2D, Deleter>;

		static ColliderUniquePtr MakeCollider();

	public:
		static void BeginScene();
		static void EndScene();
	private:
		static bool TestCollision(Collider2D* a, Collider2D* b);
	};
}
