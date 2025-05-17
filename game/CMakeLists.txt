# Game CMakeLists.txt, Author: Alessandro Manzini
# Game project configuration specifics.
#
project(bubble_bobble)

# set the target and platform
if(EMSCRIPTEN)
    set(TARGET_NAME ${PROJECT_NAME}_web)
    set(TARGET_PARAM )
elseif(APPLE)
    set(TARGET_NAME ${PROJECT_NAME})
    set(TARGET_PARAM )
else()
    set(TARGET_NAME ${PROJECT_NAME})
    set(TARGET_PARAM )
endif()

# set minigin as the default startup project
if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
endif()

add_executable(${TARGET_NAME} ${TARGET_PARAM} 
        "src/main.cpp"

        "src/component/RotatingComponent.cpp"
        "src/component/BubStateComponent.cpp"
        "src/component/FPS.cpp"

        "src/controller/DebugController.cpp"
        "src/controller/CharacterController.cpp"

        "src/registration/input.cpp"
        "src/registration/audio.cpp"

        "src/state/SpriteState.cpp"

        "src/ui_component/AudioTestUIComponent.cpp"
        "src/ui_component/DemoUIComponent.cpp"
        "src/ui_component/ObserverTestUIComponent.cpp" )

# set maximum warning level and treat warnings as errors
include(set_warning_level)

# link library
target_link_libraries(${PROJECT_NAME}
		PRIVATE engine )

# include program directories
target_include_directories(${PROJECT_NAME}
		PRIVATE "include"
		PRIVATE "src"
		PRIVATE "src/components"
		PRIVATE "src/controllers"
        PRIVATE "src/registration"
		PRIVATE "src/ui_components" )

# copy the data directory to the build directory
include(reload_resources_macro)

set(RESOURCES_SRC_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(RESOURCES_DST_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/resources")
reload_resources(${PROJECT_NAME} ${RESOURCES_SRC_DIRECTORY} ${RESOURCES_DST_DIRECTORY})

