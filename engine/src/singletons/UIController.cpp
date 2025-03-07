#include "UIController.h"

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "singletons/Renderer.h"
#include "BaseUIComponent.h"

// +--------------------------------+
// | IMGUI HEADERS					|
// +--------------------------------+
#include "imgui.h"
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

namespace engine
{
	UIController& UI_CONTROLLER = UIController::get_instance( );

	void UIController::init( )
	{
		IMGUI_CHECKVERSION( );
		ImGui::CreateContext( );
		ImGui_ImplSDL2_InitForOpenGL( RENDERER.window_ptr_, SDL_GL_GetCurrentContext( ) );
		ImGui_ImplOpenGL3_Init( );
	}

	void UIController::destroy( )
	{
		ImGui_ImplOpenGL3_Shutdown( );
		ImGui_ImplSDL2_Shutdown( );
		ImGui::DestroyContext( );
	}

	void UIController::process_input( const SDL_Event& event )
	{
		ImGui_ImplSDL2_ProcessEvent( &event );
	}

	void UIController::update( )
	{
		for ( const auto& [key, uiComponent] : ui_components_ )
		{
			uiComponent->update( );
		}
	}

	void UIController::render( )
	{
		start_imgui_render( );
		for ( const auto& [key, uiComponent] : ui_components_ )
		{
			uiComponent->render( );
		}
		end_imgui_render( );
	}

	void UIController::cleanup( )
	{
		if ( deleter_.is_cleanup_needed( ) )
		{
			deleter_.cleanup( ui_components_ );
		}
	}

	void UIController::remove_ui_component( BaseUIComponent& uiComponent )
	{
		deleter_.mark_element_for_deletion( &uiComponent );
	}

	void UIController::start_imgui_render( ) const
	{
		ImGui_ImplOpenGL3_NewFrame( );
		ImGui_ImplSDL2_NewFrame( );
		ImGui::NewFrame( );
	}

	void UIController::end_imgui_render( ) const
	{
		ImGui::Render( );
		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData( ) );
	}

}
