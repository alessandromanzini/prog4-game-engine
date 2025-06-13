#include "StatsDisplayComponent.h"

#include <framework/GameObject.h>
#include <framework/component/TextComponent.h>

#include "ScoreComponent.h"


namespace game
{
    StatsDisplayComponent::StatsDisplayComponent( owner_t& owner, std::shared_ptr<engine::Font> font )
        : Component( owner )
        , font_ptr_{ std::move( font ) }
    {
        score_text_object_ptr_ = &get_owner( ).create_child( );
        lives_text_object_ptr_ = &get_owner( ).create_child( );
        lives_text_object_ptr_->set_local_transform( engine::Transform::from_translation( { 0.f, 25.f } ) );
    }


    void StatsDisplayComponent::notify( const engine::UID event, engine::Subject*,
                                        const engine::event::broadcast_value_variant_t value )
    {
        switch ( event.uid )
        {
            case engine::UID( ScoreEvents::SCORE_INCREASED ).uid:
                update_score_text( std::get<int>( value ) );
                break;
            case engine::UID( ScoreEvents::PLAYER_DEATH ).uid:
                update_lives_text( std::get<int>( value ) );
                break;
            default:
                return;
        }
    }


    void StatsDisplayComponent::update_score_text( const int score ) const
    {
        if ( const auto text = score_text_object_ptr_->get_component<engine::TextComponent>( ); text.has_value( ) )
        {
            text.value( ).mark_for_deletion( );
        }
        score_text_object_ptr_->add_component<engine::TextComponent>( "SCORE: " + std::to_string( score ), font_ptr_ );
    }


    void StatsDisplayComponent::update_lives_text( const int lives ) const
    {
        if ( const auto text = lives_text_object_ptr_->get_component<engine::TextComponent>( ); text.has_value( ) )
        {
            text.value( ).mark_for_deletion( );
        }
        lives_text_object_ptr_->add_component<engine::TextComponent>( "LIVES: " + std::to_string( lives ), font_ptr_ );
    }

}
