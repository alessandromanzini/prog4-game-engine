#ifndef SCORECOMMAND_H
#define SCORECOMMAND_H

#include "bindings/Command.h"
#include "components/ScoreComponent.h"

namespace game
{
	class ScoreCommand final : public engine::Command
	{
	public:
		ScoreCommand( ScoreComponent& component, int points )
			: engine::Command( )
			, component_{ component }
			, points_{ points }
		{
		}

		void execute( ) override
		{
			component_.add_score( points_ );
		}

	private:
		ScoreComponent& component_;
		const int points_;

	};

}

#endif // !SCORECOMMAND_H
