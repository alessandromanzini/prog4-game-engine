#include <components/AttackComponent.h>


using namespace engine;
namespace game
{
	AttackComponent::AttackComponent( GameObjectView& owner, const int power )
		: Component{ owner }
		, power_{ power }
	{
	}

	void AttackComponent::SetTarget( const GameObject* target )
	{
		health_target_ptr_ = &target->get_component<HealthComponent>( );
	}

	void AttackComponent::Attack( ) const
	{
		health_target_ptr_->damage( power_ );
	}

}
