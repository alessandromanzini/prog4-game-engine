#include "BubbleCaptureComponent.h"

#include <framework/GameObject.h>
#include <singleton/Renderer.h>


namespace game
{
    BubbleCaptureComponent::BubbleCaptureComponent( owner_t& owner, const BubbleCaptureResources& resources )
        : Component{ owner }
        , resources_{ resources } { }


    const std::string& BubbleCaptureComponent::get_fruit_texture_path( ) const
    {
        return resources_.fruit_texture_path;
    }


    int BubbleCaptureComponent::get_fruit_value( ) const
    {
        return resources_.fruit_value;
    }

}
