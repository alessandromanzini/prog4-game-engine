#ifndef ATTACKCOMPONENT_H
#define ATTACKCOMPONENT_H

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+
#include "components/Component.h"
#include "components/HealthComponent.h"

namespace game
{
	class AttackComponent final : public engine::Component
	{
	public:
		AttackComponent( engine::GameObjectView& owner, int power );

		void SetTarget( engine::GameObject* target );

		void Attack( );

	private:
		const int power_{};

		HealthComponent* health_target_ptr_{};

	};

}

#endif // !ATTACKCOMPONENT_H
