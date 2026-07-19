cmake_minimum_required(VERSION 3.22)
project(BounceGame LANGUAGES CXX)

include(cmake/DevTools.cmake)
configure_dev_environment()

include(FetchContent)
set(SFML_BUILD_AUDIO ON CACHE BOOL "Build SFML Audio" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "Build SFML Network" FORCE)
FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 3.1.0
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(SFML)

file(
  GLOB_RECURSE PROJECT_SOURCES
  CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
)

add_executable(BounceGame ${PROJECT_SOURCES})
target_link_libraries(
  BounceGame
  PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)
target_compile_features(BounceGame PRIVATE cxx_std_20)
enable_project_warnings(BounceGame)

add_custom_command(
  TARGET BounceGame
  POST_BUILD
  COMMAND
    "${CMAKE_COMMAND}" -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:BounceGame>/assets"
  COMMENT "Copying game assets"
)

