#include "LevelLoaderComponent.h"

#include <framework/GameObject.h>
#include <framework/component/TextureComponent.h>
#include <framework/component/physics/BoxColliderComponent.h>
#include <helpers/CsvReader.h>
#include <registration/object_initializers.h>
#include <registration/tags.h>
#include <singleton/GameInstance.h>
#include <singleton/GameTime.h>
#include <singleton/ResourceManager.h>
#include <singleton/ScenePool.h>

#include "CharacterComponent.h"


namespace game
{
    LevelLoaderComponent::LevelLoaderComponent( owner_t& owner, engine::GameObject& score,
                                                std::vector<engine::GameObject*> players,
                                                std::vector<const char*> levelPaths )
        : Component{ owner }
        , level_paths_{ std::move( levelPaths ) }
        , score_object_ref_{ score }
        , players_{ std::move( players ) }
    {
        auto& scene               = engine::SCENE_POOL.get_active_scene( );
        current_level_object_ptr_ = &scene.create_object( );
        load_level( level_paths_[current_level_], *current_level_object_ptr_ );
        reposition_players( );
    }


    void LevelLoaderComponent::tick( )
    {
        if ( next_level_object_ptr_ != nullptr )
        {
            const glm::vec2 translation{ 0.f, -LEVEL_TRANSITION_SPEED_ * engine::GAME_TIME.get_delta_time( ) };
            current_level_object_ptr_->get_transform_operator( ).translate( translation );
            next_level_object_ptr_->get_transform_operator( ).translate( translation );
            if ( next_level_object_ptr_->get_world_transform( ).get_position( ).y <= 0.f )
            {
                next_level_object_ptr_->set_world_transform( {} );

                // Remove the current level object
                current_level_object_ptr_->mark_for_deletion( );
                current_level_object_ptr_ = next_level_object_ptr_;
                next_level_object_ptr_    = nullptr;
            }
        }
    }


    void LevelLoaderComponent::skip_level( )
    {
        std::set<const engine::GameObject*> found;
        while ( true )
        {
            auto enemy = engine::SCENE_POOL.get_active_scene( ).find_object(
                [&]( const engine::GameObject& object ) -> bool
                    {
                        return object.get_tag( ) == engine::UID( ObjectTags::ENEMY ) && not found.contains( &object );
                    } );
            if ( enemy )
            {
                enemy.value( ).mark_for_deletion( );
                found.insert( &enemy );
            }
            else
            {
                break;
            }
        }
    }


    void LevelLoaderComponent::load_level( const std::string& path, engine::GameObject& level )
    {
        CsvReader reader{ engine::RESOURCE_MANAGER.get_data_path( ) / path };

        auto& platforms = level.create_child( );
        platforms.set_tag( engine::UID( ObjectTags::PLATFORM ) );

        while ( not reader.eof( ) )
        {
            switch ( reader.get_int( 0 ) )
            {
                case 0:
                    if ( players_.size( ) > 0 )
                    {
                        auto& character = players_[0]->get_component<CharacterComponent>( ).value( );
                        character.set_start_position( { reader.get_float( 1 ), reader.get_float( 2 ) } );
                    }
                    break;

                case 1:
                    if ( players_.size( ) > 1 )
                    {
                        auto& character = players_[1]->get_component<CharacterComponent>( ).value( );
                        character.set_start_position( { reader.get_float( 1 ), reader.get_float( 2 ) } );
                    }
                    break;

                case 2:
                    level.add_component<engine::TextureComponent>( reader.get( 1 ) );
                    break;

                case 3:
                    level.add_component<engine::BoxColliderComponent>(
                        glm::vec2( reader.get_float( 1 ), reader.get_float( 2 ) ),
                        glm::vec2( reader.get_float( 3 ), reader.get_float( 4 ) ) );
                    break;

                case 4:
                    platforms.add_component<engine::BoxColliderComponent>(
                        glm::vec2( reader.get_float( 1 ), reader.get_float( 2 ) ),
                        glm::vec2( reader.get_float( 3 ), reader.get_float( 4 ) ) );
                    break;

                case 5:
                {
                    auto& zenchan = level.create_child( );
                    create_zenchan( zenchan, { reader.get_float( 1 ), reader.get_float( 2 ) }, players_ );
                    zenchan.on_deletion.bind( this, &LevelLoaderComponent::lower_enemy_count );
                    ++enemy_count_;
                    break;
                }

                case 6:
                {
                    auto& maita = level.create_child( );
                    create_maita( maita, { reader.get_float( 1 ), reader.get_float( 2 ) }, players_ );
                    maita.on_deletion.bind( this, &LevelLoaderComponent::lower_enemy_count );
                    ++enemy_count_;
                    break;
                }

                default:
                    printf( "Unknown level object type: %d\n", reader.get_int( 0 ) );
            }

            reader.next( );
        }
    }


    void LevelLoaderComponent::reposition_players( ) const
    {
        for ( const auto player : players_ )
        {
            auto& character = player->get_component<CharacterComponent>( ).value( );
            character.reposition( );
        }
    }


    void LevelLoaderComponent::lower_enemy_count( )
    {
        assert( enemy_count_ != 0 );
        --enemy_count_;
        if ( enemy_count_ == 0 )
        {
            load_next_level( );
        }
    }


    void LevelLoaderComponent::load_next_level( )
    {
        std::function createLevel = [this]
            {
                auto& scene = engine::SCENE_POOL.get_active_scene( );

                next_level_object_ptr_ = &scene.create_object( );
                next_level_object_ptr_->set_world_transform( engine::Transform::from_translation( {
                    0.f, engine::GAME_INSTANCE.get_screen_dimensions( ).y
                } ) );

                current_level_ = ( current_level_ + 1 ) % level_paths_.size( );
                load_level( level_paths_[current_level_], *next_level_object_ptr_ );
                reposition_players( );
            };
        engine::GAME_TIME.set_timeout( LEVEL_PERSISTANCE_SECONDS_, std::move( createLevel ) );
    }

}
