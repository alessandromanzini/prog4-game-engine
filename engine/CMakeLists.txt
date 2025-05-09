# Engine CMakeLists.txt, Author: Alessandro Manzini
# Engine library configuration specifics.
#
project(engine)

# create library target
add_library(${PROJECT_NAME}
		"src/bindings/binding_controls.cpp"
		"src/bindings/binding_device.cpp"
		"src/bindings/InputBuffer.cpp"
        "src/bindings/InputMappingContext.cpp"

		"include/public/controllers/BaseController.h"
		"src/controllers/PlayerController.cpp"

		"include/public/event/EventQueue.h"
        "src/event/Subject.cpp"

        "include/public/framework/Deleter.h"
        "src/framework/GameObject.cpp"
        "src/framework/GameObjectView.cpp"
        "src/framework/Scene.cpp"
        "src/framework/Transform.cpp"

		"src/framework/components/AudioComponent.cpp"
		"src/framework/components/Component.cpp"
        "src/framework/components/TextComponent.cpp"
        "src/framework/components/TextureComponent.cpp"
        "src/framework/ui_components/InputDisplayUIComponent.cpp"
		"src/framework/ui_components/UIComponent.cpp"

	    "src/framework/resources/Font.cpp"
        "src/framework/resources/Texture2D.cpp"
		"src/framework/resources/sound/Audio.cpp"
		"src/framework/resources/sound/SdlAudio.cpp"

        "src/core/Minigin.cpp"

		"include/public/services/service_info.h"
		"src/services/sound/SdlSoundSystem.cpp"
		"src/services/sound/SoundSystemLogger.cpp"
		"src/services/sound/ParallelSoundSystem.cpp"

        "src/singletons/GameTime.cpp"
        "src/singletons/InputSystem.cpp"
        "src/singletons/Renderer.cpp"
        "src/singletons/ResourceManager.cpp"
        "src/singletons/ScenePool.cpp"
        "src/singletons/UIController.cpp"
		"src/singletons/ServiceLocator.cpp"
		src/framework/components/SpriteComponent.cpp
		include/public/framework/components/SpriteComponent.h)

# include platform specific sources
if (APPLE)
	target_sources(${PROJECT_NAME} PRIVATE "src/core/imgui_core_dwn.cpp")
else()
	target_sources(${PROJECT_NAME} PRIVATE "src/core/imgui_core_win.cpp")
endif()

# include directories
target_include_directories(${PROJECT_NAME}
		PUBLIC 	"include/public"
		PRIVATE "include/private"
		PRIVATE "src" )

# set maximum warning level and treat warnings as errors
include(set_warning_level)

# add glm
include(glm_fetchcontent)

# add SDL2
if (WIN32)
    include(sdl_win_fetchcontent)

elseif(NOT EMSCRIPTEN)
    include(sdl_dwn_fetchcontent)

endif()

# include vld
find_package(VLD CONFIG)
target_link_libraries(${PROJECT_NAME} PUBLIC ${VLD_LIBRARY})

# add debug-only dependencies
# if (CMAKE_BUILD_TYPE STREQUAL "Debug")
# add imgui
include(imgui_fetchcontent)

# setup the target properties
if(EMSCRIPTEN)
    include(setup_emscripten_toolchain)
  
elseif(WIN32)
  set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${GAME_BINARY_PATH})

  # add post build commands
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_LIBRARY_DLL} ${GAME_BINARY_PATH} )
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_IMAGE_LIBRARY_DLL} ${GAME_BINARY_PATH} )
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_TTF_LIBRARY_DLL} ${GAME_BINARY_PATH} )
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_MIXER_LIBRARY_DLL} ${GAME_BINARY_PATH} )
endif()
