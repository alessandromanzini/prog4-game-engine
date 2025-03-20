include(FetchContent)

FetchContent_Declare(
  glm
  URL https://github.com/g-truc/glm/releases/download/1.0.1/glm-1.0.1-light.zip
  DOWNLOAD_NO_PROGRESS ON
  DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)

FetchContent_MakeAvailable(glm)
target_include_directories(${PROJECT_NAME} PUBLIC ${glm_SOURCE_DIR})
