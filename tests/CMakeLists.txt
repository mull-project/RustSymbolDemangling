set(rustdemangle_tests_sources
  rustdemangle_test.cpp
)

add_executable(rustdemangle_tests ${rustdemangle_tests_sources})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(rustdemangle_tests
  rustdemangle

  ${RUSTDEMANGLE_DEPENDENCY_GOOGLETEST}
  ${RUSTDEMANGLE_DEPENDENCY_GOOGLETESTMAIN}
)

# # This copies (symlinks) fixtures/*.bc files that used by unit tests.
# add_custom_command(TARGET MullUnitTests POST_BUILD
#                    COMMAND ${CMAKE_COMMAND} -E create_symlink
#                    ${CMAKE_CURRENT_LIST_DIR}/fixtures $<TARGET_FILE_DIR:MullUnitTests>/fixtures)
