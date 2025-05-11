#ifndef SPRITESELECTORCOMPONENT_H
#define SPRITESELECTORCOMPONENT_H

#include <framework/behaviour/fsm/FiniteStateMachine.h>
#include <framework/behaviour/fsm/sprite_transitions.h>
#include <framework/components/Component.h>
#include <framework/resources/data/Blackboard.h>
#include <framework/resources/texture/Sprite2D.h>


namespace engine
{
    class SpriteSelectorComponent final : public Component
    {
    public:
        explicit SpriteSelectorComponent( owner_t& owner );

        void tick( ) override;
        void render( ) const override;

        void add_sprite( UID uid, Sprite2D&& sprite );

        template <typename condition_t> requires std::derived_from<condition_t, fsm::Condition>
        void make_transition( UID from, UID to );

    private:
        Blackboard blackboard_{};
        std::unique_ptr<FiniteStateMachine> state_machine_ptr_{ nullptr };

        std::map<UID, std::unique_ptr<fsm::State>> sprite_states_{};
        std::vector<std::unique_ptr<fsm::Condition>> conditions_{};

    };


    template <typename condition_t> requires std::derived_from<condition_t, fsm::Condition>
    void SpriteSelectorComponent::make_transition( const UID from, const UID to )
    {
        assert( sprite_states_.contains( from ) && sprite_states_.contains( to )
            && "SpriteSelectorComponent::make_transition: Invalid sprite UID!" );

        // Lazy initialization of the state machine to assign the first state
        if ( state_machine_ptr_ == nullptr )
        {
            state_machine_ptr_ = std::make_unique<FiniteStateMachine>( sprite_states_.at( from ).get( ), blackboard_ );
        }

        state_machine_ptr_->add_transition( sprite_states_.at( from ).get( ),
                                            sprite_states_.at( to ).get( ),
                                            conditions_.emplace_back( std::make_unique<condition_t>( blackboard_ ) ).get( ) );
    }

}


#endif //!SPRITESELECTORCOMPONENT_H
