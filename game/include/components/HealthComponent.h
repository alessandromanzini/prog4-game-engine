#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "components/Component.h"
#include "event/Subject.h"


namespace game
{
	class HealthComponent final : public engine::Component, public engine::Subject
	{
	public:
		HealthComponent( engine::GameObjectView& owner, int health );

		[[nodiscard]] int get_health( ) const;
		[[nodiscard]] bool is_alive( ) const;

		void damage( int amount );

	private:
		int health_{};

	};

}


#endif //!HEALTHCOMPONENT_H
