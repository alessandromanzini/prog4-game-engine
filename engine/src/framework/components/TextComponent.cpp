#include <framework/components/TextComponent.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/GameObject.h>
#include <singletons/Renderer.h>

#include <framework/resources/Font.h>
#include <framework/resources/texture/Texture2D.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <SDL_ttf.h>
#include <stdexcept>


namespace engine
{
    TextComponent::TextComponent( owner_t& owner, const std::string& text, std::shared_ptr<Font> font )
        : Component( owner )
        , font_ptr_{ std::move( font ) }
    {
        set_text( text );

        if ( font_ptr_ == nullptr )
        {
            throw std::runtime_error( "Font cannot be nullptr!" );
        }
    }


    void TextComponent::tick( )
    {
        if ( needs_update_ )
        {
            constexpr SDL_Color color = { 255, 255, 255, 255 }; // only white text is supported now
            const auto surf           = TTF_RenderText_Blended( font_ptr_->get_font( ), text_.c_str( ), color );
            if ( surf == nullptr )
            {
                throw std::runtime_error( std::string( "Render text failed: " ) + SDL_GetError( ) );
            }

            auto texture = SDL_CreateTextureFromSurface( RENDERER.get_SDL_renderer( ), surf );
            if ( texture == nullptr )
            {
                throw std::runtime_error(
                    std::string( "Create text texture from surface failed: " ) + SDL_GetError( ) );
            }

            SDL_FreeSurface( surf );
            text_texture_ptr_ = std::make_shared<Texture2D>( texture );
            needs_update_     = false;
        }
    }


    void TextComponent::render( ) const
    {
        if ( text_texture_ptr_ != nullptr )
        {
            const auto pos = get_owner( ).get_world_transform( ).get_position( );
            RENDERER.render_texture( *text_texture_ptr_, pos );
        }
    }


    void TextComponent::set_text( const std::string& text )
    {
        if ( !text.empty( ) )
        {
            text_         = text;
            needs_update_ = true;
        }
    }

}
