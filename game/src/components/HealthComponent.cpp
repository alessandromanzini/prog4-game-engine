#include "components/HealthComponent.h"

using namespace engine;

namespace game
{
	HealthComponent::HealthComponent( GameObjectView& owner, int health )
		: Component( owner )
		, health_{ health }
	{
	}

	int HealthComponent::get_health( ) const
	{
		return health_;
	}

	bool HealthComponent::is_alive( ) const
	{
		return health_ > 0;
	}

	void HealthComponent::damage( int amount )
	{
		if ( not is_alive() )
		{
			return;
		}

		health_ -= amount;
		broadcast( UID( "ActorHealthChanged" ), health_ );
		if ( not is_alive( ) )
		{
			broadcast( UID( "ActorDead" ) );
		}
	}

}
