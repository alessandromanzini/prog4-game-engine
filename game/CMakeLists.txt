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
    set(TARGET_PARAM APPLE)
else()
    set(TARGET_NAME ${PROJECT_NAME})
    set(TARGET_PARAM WIN32)
endif()

# set minigin as the default startup project
if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
endif()

add_executable(${TARGET_NAME} ${TARGET_PARAM} 
        "src/main.cpp"
        "src/components/RotatingComponent.cpp"  
        "src/components/MoveComponent.cpp"  
        "src/components/DemoUIComponent.cpp"  
        "src/bindings/InputUtils.cpp" )

# link library
target_link_libraries(${PROJECT_NAME}
		PRIVATE engine )

# include program directories
target_include_directories(${PROJECT_NAME}
		PRIVATE "./include"
		# PRIVATE "./include/singletons"
		PRIVATE "./include/compoments"
		PRIVATE "./include/bindings"
		PRIVATE "./src"
        # PRIVATE "./src/singletons"
		PRIVATE "./src/compoments"
        PRIVATE "./src/bindings" )

# copy the data directory to the build directory
set(RESOURCES_SRC_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(RESOURCES_DST_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/resources")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory_if_different ${RESOURCES_SRC_DIRECTORY} ${RESOURCES_DST_DIRECTORY} )
