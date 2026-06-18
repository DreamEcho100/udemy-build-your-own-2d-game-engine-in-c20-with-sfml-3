include_guard(GLOBAL)

function(configure_dev_environment)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" PARENT_SCOPE)

  if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    return()
  endif()

  set(database "${CMAKE_BINARY_DIR}/compile_commands.json")
  set(link "${CMAKE_SOURCE_DIR}/compile_commands.json")

  file(CREATE_LINK "${database}" "${link}" SYMBOLIC RESULT link_result)
  if(NOT link_result STREQUAL "0")
    message(
      STATUS
      "Could not create compile_commands.json symlink (${link_result}); "
      "the Debug build will copy it instead."
    )
    set(DEVTOOLS_COPY_COMPILE_COMMANDS ON PARENT_SCOPE)
    set(DEVTOOLS_COMPILE_DATABASE_SOURCE "${database}" PARENT_SCOPE)
    set(DEVTOOLS_COMPILE_DATABASE_DESTINATION "${link}" PARENT_SCOPE)
  endif()
endfunction()

function(enable_project_warnings target)
  if(MSVC)
    target_compile_options("${target}" PRIVATE /W4 /permissive-)
  else()
    target_compile_options("${target}" PRIVATE -Wall -Wextra -Wpedantic)
  endif()

  if(DEVTOOLS_COPY_COMPILE_COMMANDS)
    add_custom_command(
      TARGET "${target}"
      POST_BUILD
      COMMAND
        "${CMAKE_COMMAND}" -E copy_if_different
        "${DEVTOOLS_COMPILE_DATABASE_SOURCE}"
        "${DEVTOOLS_COMPILE_DATABASE_DESTINATION}"
      COMMENT "Copying compile_commands.json for clangd"
      VERBATIM
    )
  endif()
endfunction()
