file(GLOB example_sources CONFIGURE_DEPENDS *.cpp)

set(EXAMPLE_EXECUTABLES "")
set(EXAMPLE_TARGETS "")

foreach(src ${example_sources})
  get_filename_component(name ${src} NAME_WE)
  set(alias_target "example-${name}")

  # Define the real executable
  add_executable(${name} ${src})
  target_link_libraries(${name} PRIVATE tf::trueform)

  # Define a lightweight alias target: 'make example-foo'
  add_custom_target(${alias_target} DEPENDS ${name})
  list(APPEND EXAMPLE_TARGETS ${alias_target})
  list(APPEND EXAMPLE_EXECUTABLES ${name})
endforeach()

# Group target to build all executables: 'make examples'
add_custom_target(examples DEPENDS ${EXAMPLE_EXECUTABLES})

# Copy compile_commands.json only after 'make examples'
add_custom_command(
  TARGET examples POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
          ${CMAKE_BINARY_DIR}/compile_commands.json
          ${CMAKE_SOURCE_DIR}/compile_commands.json
  COMMENT "Copying compile_commands.json to project root after building examples"
)
