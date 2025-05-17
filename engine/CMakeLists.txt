# Engine CMakeLists.txt, Author: Alessandro Manzini
# Engine library configuration specifics.
#
project(engine)

# create library target
add_library(${PROJECT_NAME}
		"include/public/framework/behaviour/fsm/transitions.h"
		"include/public/framework/behaviour/fsm/__transition/base_transitions.h"
		"src/framework/behaviour/fsm/FiniteStateMachine.cpp"
		"src/framework/behaviour/fsm/__transition/conditions/RigidBodyCondition.cpp"

		"src/framework/binding/binding_controls.cpp"
		"src/framework/binding/binding_device.cpp"
		"src/framework/binding/InputBuffer.cpp"
        "src/framework/binding/InputMappingContext.cpp"

		"include/public/framework/controller/BaseController.h"
		"src/framework/controller/PlayerController.cpp"

		"include/public/framework/event/EventQueue.h"
        "src/framework/event/Subject.cpp"

        "include/public/framework/resource/data/Deleter.h"
        "src/framework/GameObject.cpp"
        "src/framework/GameObjectView.cpp"
        "src/framework/Scene.cpp"
        "src/framework/Transform.cpp"

		"src/framework/component/AudioComponent.cpp"
		"src/framework/component/Component.cpp"
        "src/framework/component/TextComponent.cpp"
        "src/framework/component/TextureComponent.cpp"
		"src/framework/component/physics/RigidBodyComponent.cpp"
		"src/framework/ui_component/InputDisplayUIComponent.cpp"
		"src/framework/ui_component/UIComponent.cpp"

		"include/public/framework/resource/data/Blackboard.h"
		"include/public/framework/resource/data/blackboard_ids.h"
	    "src/framework/resource/Font.cpp"
		"src/framework/resource/texture/Sprite2D.cpp"
        "src/framework/resource/texture/Texture2D.cpp"
		"src/framework/resource/sound/Audio.cpp"
		"src/framework/resource/sound/SdlAudio.cpp"

        "src/core/Minigin.cpp"

		"include/public/service/service_info.h"
		"src/service/sound/SdlSoundSystem.cpp"
		"src/service/sound/SoundSystemLogger.cpp"
		"src/service/sound/ParallelSoundSystem.cpp"

		"src/singleton/GameInstance.cpp"
        "src/singleton/GameTime.cpp"
        "src/singleton/InputSystem.cpp"
        "src/singleton/Renderer.cpp"
        "src/singleton/ResourceManager.cpp"
        "src/singleton/ScenePool.cpp"
        "src/singleton/UIController.cpp"
		"src/singleton/ServiceLocator.cpp"
		src/framework/component/physics/BoxColliderComponent.cpp
		include/public/framework/component/physics/BoxColliderComponent.h
		src/framework/component/physics/ColliderComponent.cpp
		include/public/framework/component/physics/ColliderComponent.h
		include/public/framework/event/Dispatcher.h
)

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
