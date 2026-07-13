cmake_minimum_required(VERSION 3.22)
project(Time LANGUAGES CXX)

include(cmake/DevTools.cmake)
configure_dev_environment()

include(FetchContent)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "Build SFML Audio" FORCE)
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

add_executable(Time ${PROJECT_SOURCES})
target_link_libraries(
  Time
  PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)
target_compile_features(Time PRIVATE cxx_std_20)
enable_project_warnings(Time)
