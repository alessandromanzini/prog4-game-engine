#include <framework/components/SpriteSelectorComponent.h>

#include <framework/GameObject.h>
#include <framework/components/RigidBodyComponent.h>
#include <framework/resources/data/blackboard_ids.h>


namespace engine
{
    SpriteSelectorComponent::SpriteSelectorComponent( owner_t& owner )
        : Component{ owner }
    {
        blackboard_.store( ids::OBJECT_UID, &owner );
        blackboard_.store( ids::RIGID_BODY_UID, &owner.get_component<RigidBodyComponent>( ) );
    }


    void SpriteSelectorComponent::tick( )
    {
        if ( state_machine_ptr_ )
        {
            state_machine_ptr_->tick( );
        }
    }


    void SpriteSelectorComponent::render( ) const
    {
        if ( state_machine_ptr_ )
        {
            state_machine_ptr_->render( );
        }
    }


    void SpriteSelectorComponent::add_sprite( UID uid, Sprite2D&& sprite )
    {
        sprite_states_.insert( std::make_pair( uid, std::make_unique<fsm::states::SpriteState>( std::move( sprite ) ) ) );
    }

}
