#include "components/ScoreComponent.h"


using namespace engine;

namespace game
{
	ScoreComponent::ScoreComponent( engine::GameObjectView& owner )
		: engine::Component( owner )
	{
	}

	void ScoreComponent::add_score( const int score )
	{
		if ( score_ >= WINNING_SCORE_ )
		{
			return;
		}

		score_ += score;
		broadcast( UID( "ActorScoreChanged" ), score_ );
		if ( score_ >= WINNING_SCORE_ )
		{
			broadcast( UID( "ActorWon" ) );
		}
	}

	int ScoreComponent::get_score( ) const
	{
		return score_;
	}

}
