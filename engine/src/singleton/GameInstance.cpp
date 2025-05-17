#include <singleton/GameInstance.h>


namespace engine
{
    GameInstance& GAME_INSTANCE = GameInstance::get_instance( );


    void GameInstance::remove_player_controller( const PlayerController& controller )
    {
        std::erase_if( player_controllers_, [&]( const auto& controllerPtr )
                           {
                               return controllerPtr.get( ) == &controller;
                           } );
    }


    void GameInstance::set_gravity_coefficient( const float coeffiecient )
    {
        gravity_coefficient_ = coeffiecient;
    }


    float GameInstance::get_gravity_coefficient( ) const
    {
        return gravity_coefficient_;
    }


    void GameInstance::destroy( )
    {
        player_controllers_.clear( );
    }

}
