#ifndef ATTACKCOMMAND_H
#define ATTACKCOMMAND_H

#include "bindings/Command.h"
#include "components/AttackComponent.h"

namespace game
{
	class AttackCommand final : public engine::Command
	{
	public:
		AttackCommand( AttackComponent& component )
			: engine::Command()
			, component_{ component }
		{
		}

		void execute( ) override
		{
			component_.Attack( );
		}

	private:
		AttackComponent& component_;

	};

}

#endif // !ATTACKCOMMAND_H
