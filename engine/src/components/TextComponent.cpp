#include "components/TextComponent.h"

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+
#include "singletons/Renderer.h"
#include "Font.h"
#include "Texture2D.h"

// +--------------------------------+
// | Standard Headers				|
// +--------------------------------+
#include <stdexcept>
#include <SDL_ttf.h>

using namespace engine;

TextComponent::TextComponent( GameObjectView& owner, const std::string& text, std::shared_ptr<Font> pFont )
	: BaseComponent( owner )
	, font_ptr_{ std::move( pFont ) }
	, text_texture_ptr_{ nullptr }
{
	set_text( text );

	if ( font_ptr_ == nullptr )
	{
		throw std::runtime_error( "Font cannot be nullptr!" );
	}
}

void TextComponent::update( )
{
	if ( needs_update_ )
	{
		const SDL_Color color = { 255, 255, 255, 255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended( font_ptr_->get_font( ), text_.c_str( ), color );
		if ( surf == nullptr )
		{
			throw std::runtime_error( std::string( "Render text failed: " ) + SDL_GetError( ) );
		}

		auto texture = SDL_CreateTextureFromSurface( Renderer::get_instance( ).get_SDL_renderer( ), surf );
		if ( texture == nullptr )
		{
			throw std::runtime_error( std::string( "Create text texture from surface failed: " ) + SDL_GetError( ) );
		}

		SDL_FreeSurface( surf );
		text_texture_ptr_ = std::make_shared<Texture2D>( texture );
		needs_update_ = false;
	}
}

void TextComponent::render( ) const
{
	if ( text_texture_ptr_ != nullptr )
	{
		const auto pos = get_owner( ).get_world_transform( ).get_position( );
		Renderer::get_instance( ).render_texture( *text_texture_ptr_, pos.x, pos.y );
	}
}

void TextComponent::set_text( const std::string& text )
{
	if ( !text.empty( ) )
	{
		text_ = text;
		needs_update_ = true;
	}
}
