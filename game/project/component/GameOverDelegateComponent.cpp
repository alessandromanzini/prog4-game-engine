#include "GameOverDelegateComponent.h"

#include "ScoreComponent.h"


namespace game
{
    GameOverDelegateComponent::GameOverDelegateComponent( owner_t& owner ) : Component{ owner } { }


    void GameOverDelegateComponent::add_observer( engine::Observer& observer )
    {
        subject_.add_observer( observer );
    }


    void GameOverDelegateComponent::remove_observer( const engine::Observer& observer )
    {
        subject_.remove_observer( observer );
    }


    void GameOverDelegateComponent::signal_gameover( ) const
    {
        subject_.broadcast( engine::UID( ScoreEvents::GAMEOVER ), 0 );
    }

}
