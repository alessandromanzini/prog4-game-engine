#include <framework/resources/sound/Audio.h>

#include <singletons/ResourceManager.h>


namespace engine
{
    UID Audio::get_sound_id( ) const
    {
        return sound_id_;
    }


    UID Audio::get_tag_id( ) const
    {
        return tag_id_;
    }


    sound::SoundType Audio::get_type( ) const
    {
        return type_;
    }


    Audio::Audio( const sound::SoundType type, const UID soundId, const UID tagId )
        : type_{ type }
        , sound_id_{ soundId }
        , tag_id_{ tagId } { }

}
