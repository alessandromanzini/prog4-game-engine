#include "components/MoveComponent.h"

#include "singletons/GameTime.h"
#include "singletons/InputSystem.h"
#include "bindings/InputUtils.h"
#include "bindings/MoveCommand.h"

namespace game
{
	MoveComponent::MoveComponent( engine::GameObjectView& owner, float movementSpeed, bool /* gamepad */ )
		: Component{ owner }
		, move_command_ptr_{ std::make_unique<MoveCommand>( *this ) }
		, movement_speed_{ movementSpeed }
	{
		auto& imc{ engine::INPUT_SYSTEM.get_input_mapping_context( ) };

		imc.bind_input_action( engine::UID( IA::Move ), move_command_ptr_.get() );

		/*if ( gamepad )
		{
			imc.bind_input_action<IABMove>( code_to_mask( BTN_MASK_CODE::BTN_DPAD_UP ), std::bind( &MoveComponent::move, this, glm::vec2{ 0.f, -1.f }, std::placeholders::_1 ) );
			imc.bind_input_action<IABMove>( code_to_mask( BTN_MASK_CODE::BTN_DPAD_DOWN ), std::bind( &MoveComponent::move, this, glm::vec2{ 0.f, 1.f }, std::placeholders::_1 ) );
			imc.bind_input_action<IABMove>( code_to_mask( BTN_MASK_CODE::BTN_DPAD_LEFT ), std::bind( &MoveComponent::move, this, glm::vec2{ -1.f, 0.f }, std::placeholders::_1 ) );
			imc.bind_input_action<IABMove>( code_to_mask( BTN_MASK_CODE::BTN_DPAD_RIGHT ), std::bind( &MoveComponent::move, this, glm::vec2{ 1.f, 0.f }, std::placeholders::_1 ) );
		}
		else
		{
			imc.bind_input_action<IABMove>( SDLK_w, std::bind( &MoveComponent::move, this, glm::vec2{ 0.f, -1.f }, std::placeholders::_1 ) );
			imc.bind_input_action<IABMove>( SDLK_s, std::bind( &MoveComponent::move, this, glm::vec2{ 0.f, 1.f }, std::placeholders::_1 ) );
			imc.bind_input_action<IABMove>( SDLK_a, std::bind( &MoveComponent::move, this, glm::vec2{ -1.f, 0.f }, std::placeholders::_1 ) );
			imc.bind_input_action<IABMove>( SDLK_d, std::bind( &MoveComponent::move, this, glm::vec2{ 1.f, 0.f }, std::placeholders::_1 ) );
		}*/
	}

	MoveComponent::~MoveComponent( ) noexcept
	{
		auto& imc{ engine::INPUT_SYSTEM.get_input_mapping_context( ) };

		imc.unbind_input_action( engine::UID( IA::Move ), move_command_ptr_.get( ) );
	}

	void MoveComponent::update( )
	{
		if ( direction_ != glm::vec2{ 0.f, 0.f } )
		{
			// Consume direction
			const auto movement{ movement_speed_ * engine::GAME_TIME.get_delta_time( ) };
			glm::vec2 translation{ glm::normalize( direction_ ) * movement };
			direction_ = glm::vec2{ 0.f, 0.f };

			auto transform = glm::translate( get_owner( ).get_world_transform( ).get_matrix( ), translation );
			get_owner( ).set_world_transform( transform );
		}
	}

	void MoveComponent::move( glm::vec2 direction, bool )
	{
		direction_ += direction;
	}

}
