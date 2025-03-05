#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include "Singleton.h"

namespace engine
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:
		void init( SDL_Window* window );
		void render( ) const;
		void destroy( );

		void render_texture( const Texture2D& texture, float x, float y ) const;
		void render_texture( const Texture2D& texture, float x, float y, float width, float height ) const;

		[[nodiscard]] SDL_Renderer* get_SDL_renderer( ) const;

		[[nodiscard]] const SDL_Color& get_background_color( ) const
		{
			return clear_color_;
		}

		void set_background_color( const SDL_Color& color )
		{
			clear_color_ = color;
		}
		
	private:
		SDL_Renderer* renderer_ptr_{};
		SDL_Window* window_ptr_{};
		SDL_Color clear_color_{};

		void init_imgui( );
		void destroy_imgui( );
		void start_imgui_render( ) const;
		void end_imgui_render( ) const;

	};

	extern Renderer& RENDERER;

}

#endif // !RENDERER_H
