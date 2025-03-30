#include "bindings/DeviceContext.h"
#include <algorithm>


namespace engine
{
    std::optional<std::reference_wrapper<const std::vector<InputActionBinding>>> DeviceContext::get_bindings(
        const uint32_t key ) const
    {
        const auto it = action_binds_.find( key );
        if ( it == action_binds_.end( ) )
        {
            return std::nullopt;
        }
        return it->second;
    }


    std::vector<InputActionBinding>& DeviceContext::get_or_create_bindings( uint32_t key )
    {
        return action_binds_[key];
    }


    bool DeviceContext::has_bindings( const UID uid, const uint32_t key ) const
    {
        return action_binds_.contains( key ) && contains_uid_input_action( uid, action_binds_.at( key ) );
    }


    bool DeviceContext::has_bindings( const UID uid ) const
    {
        return std::ranges::any_of( action_binds_, [uid]( const auto& pair )
                                        {
                                            return contains_uid_input_action( uid, pair.second );
                                        } );
    }


    bool DeviceContext::contains_uid_input_action( const UID uid,
                                                   const std::vector<InputActionBinding>&
                                                   actions )
    {
        return std::ranges::any_of( actions, [uid]( const auto& action ) { return action.uid == uid; } );
    }
}
