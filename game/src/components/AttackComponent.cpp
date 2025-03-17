#include "components/AttackComponent.h"

#include "singletons/InputSystem.h"

using namespace engine;
namespace game
{
	AttackComponent::AttackComponent( GameObjectView& owner, int power )
		: Component{ owner }
		, power_{ power }
	{
	}

	void AttackComponent::SetTarget( GameObject* target )
	{
		health_target_ptr_ = &target->get_component<HealthComponent>( );
	}

	void AttackComponent::Attack( )
	{
		health_target_ptr_->damage( power_ );
	}

}