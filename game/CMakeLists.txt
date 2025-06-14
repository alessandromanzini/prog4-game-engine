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
        "project/main.cpp"

        "project/command/AttackCommand.cpp"
        "project/command/JumpCommand.cpp"
        "project/command/MoveCommand.cpp"

        "project/component/BubbleComponent.cpp"
        "project/component/BubbleCaptureComponent.cpp"
        "project/component/CharacterComponent.cpp"
        "project/component/CollisionsComponent.cpp"
        "project/component/FruitComponent.cpp"
        "project/component/ZenChanComponent.cpp"
        "project/component/FPS.cpp"
        "project/component/RotatingComponent.cpp"
        "project/component/EnemyAiComponent.cpp"
        "project/component/JoinMenuComponent.cpp"
        "project/component/ScoreComponent.cpp"
        "project/component/ScoreDelegateComponent.cpp"
        "project/component/StatsDisplayComponent.cpp"

        "project/controller/DebugController.cpp"
        "project/controller/CharacterController.cpp"

        "project/registration/input.cpp"
        "project/registration/audio.cpp"
        "project/registration/object_initializers.cpp"

        "project/state/__base/CharacterState.cpp"
        "project/state/character/character_conditions.cpp"
        "project/state/character/RepositionState.cpp"
        "project/command/BubbleAttackCommand.cpp"
        "project/command/EmptyAttackCommand.cpp"
        "project/registration/object_stats.cpp"
        "project/component/MaitaComponent.cpp"
        "project/command/RockAttackCommand.cpp"
        "project/component/RockComponent.cpp"
        "project/state/character/AttackState.cpp"
        "project/state/character/JumpState.cpp"
        "project/component/LevelLoaderComponent.cpp"
        "project/helpers/CsvReader.cpp"
        project/component/GameOverComponent.cpp
        project/component/GameOverComponent.h
        project/component/GameOverDelegateComponent.cpp
        project/component/GameOverDelegateComponent.h
        project/state/character/LockState.cpp
        project/state/character/LockState.h)

# set maximum warning level and treat warnings as errors
include(set_warning_level)

# link library
target_link_libraries(${PROJECT_NAME}
		PRIVATE "engine" )

# copy the data directory to the build directory
include(reload_resources_macro)

target_include_directories(${PROJECT_NAME} PRIVATE "./project" )

set(RESOURCES_SRC_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(RESOURCES_DST_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/resources")
reload_resources(${PROJECT_NAME} ${RESOURCES_SRC_DIRECTORY} ${RESOURCES_DST_DIRECTORY})

