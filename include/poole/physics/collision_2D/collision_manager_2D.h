#pragma once

#include "poole/core.h"

#include "box_collider_2D.h"

namespace Poole
{
	class ColliderManager2D
	{
	public:
		static std::vector<std::shared_ptr<Collider2D>> Colliders;
		static std::vector<std::shared_ptr<Collider2D>> CollidersToFreeAtEndOfTick;

		template<typename T, typename ... TArgs>
		static std::shared_ptr<T> MakeCollider(TArgs&& ... params)
		{
			T* ptr = new T(std::forward<TArgs>(params)...);
			std::shared_ptr<T> out = std::shared_ptr<T>(ptr);
			Colliders.push_back(out);
			return out;
		}

	public:
		static void BeginScene();
		static void EndScene();
	//private:
		static bool TestCollisionBase(const Collider2D& a, const Collider2D& b);
		static bool TestCollision(const BoxCollider2D& a, const BoxCollider2D& b);
	};
}
