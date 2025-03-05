#include "singletons/Renderer.h"

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+
#include "singletons/ScenePool.h"
#include "Texture2D.h"

#ifdef _USE_IMGUI
#include "imgui.h"
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#endif

// +--------------------------------+
// | Standard Headers				|
// +--------------------------------+
#include <stdexcept>
#include <cstring>

namespace engine
{
	Renderer& engine::RENDERER = Renderer::get_instance( );

	int GetOpenGLDriverIndex( )
	{
		auto openglIndex = -1;
		const auto driverCount = SDL_GetNumRenderDrivers( );
		for ( auto i = 0; i < driverCount; i++ )
		{
			SDL_RendererInfo info;
			if ( !SDL_GetRenderDriverInfo( i, &info ) )
			{
				if ( !strcmp( info.name, "opengl" ) )
				{
					openglIndex = i;
				}
			}
		}
		return openglIndex;
	}

	void Renderer::init( SDL_Window* window )
	{
		window_ptr_ = window;
		renderer_ptr_ = SDL_CreateRenderer( window, GetOpenGLDriverIndex( ), SDL_RENDERER_ACCELERATED );
		if ( renderer_ptr_ == nullptr )
		{
			throw std::runtime_error( std::string( "SDL_CreateRenderer Error: " ) + SDL_GetError( ) );
		}

		init_imgui( );
	}

	void Renderer::render( ) const
	{
		const auto& color = get_background_color( );

		SDL_SetRenderDrawColor( renderer_ptr_, color.r, color.g, color.b, color.a );
		SDL_RenderClear( renderer_ptr_ );

		SCENE_POOL.render( );

		start_imgui_render( );
		SCENE_POOL.render_ui( );
		end_imgui_render( );

		SDL_RenderPresent( renderer_ptr_ );
	}

	void Renderer::destroy( )
	{
		destroy_imgui( );

		if ( renderer_ptr_ != nullptr )
		{
			SDL_DestroyRenderer( renderer_ptr_ );
			renderer_ptr_ = nullptr;
		}
	}

	void Renderer::render_texture( const Texture2D& texture, const float x, const float y ) const
	{
		SDL_Rect dst{};
		dst.x = static_cast<int>( x );
		dst.y = static_cast<int>( y );
		SDL_QueryTexture( texture.get_SDL_texture( ), nullptr, nullptr, &dst.w, &dst.h );
		SDL_RenderCopy( get_SDL_renderer( ), texture.get_SDL_texture( ), nullptr, &dst );
	}

	void Renderer::render_texture( const Texture2D& texture, const float x, const float y, const float width, const float height ) const
	{
		SDL_Rect dst{};
		dst.x = static_cast<int>( x );
		dst.y = static_cast<int>( y );
		dst.w = static_cast<int>( width );
		dst.h = static_cast<int>( height );
		SDL_RenderCopy( get_SDL_renderer( ), texture.get_SDL_texture( ), nullptr, &dst );
	}

	SDL_Renderer* Renderer::get_SDL_renderer( ) const
	{
		return renderer_ptr_;
	}

	void Renderer::init_imgui( )
	{
#ifdef _USE_IMGUI
		IMGUI_CHECKVERSION( );
		ImGui::CreateContext( );
		ImGui_ImplSDL2_InitForOpenGL( window_ptr_, SDL_GL_GetCurrentContext( ) );
		ImGui_ImplOpenGL3_Init( );
#endif
	}

	void Renderer::destroy_imgui( )
	{
#ifdef _USE_IMGUI
		ImGui_ImplOpenGL3_Shutdown( );
		ImGui_ImplSDL2_Shutdown( );
		ImGui::DestroyContext( );
#endif
	}

	void Renderer::start_imgui_render( ) const
	{
#ifdef _USE_IMGUI
		ImGui_ImplOpenGL3_NewFrame( );
		ImGui_ImplSDL2_NewFrame( );
		ImGui::NewFrame( );
#endif
	}

	void Renderer::end_imgui_render( ) const
	{
#ifdef _USE_IMGUI
		ImGui::Render( );
		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData( ) );
#endif
	}
}
