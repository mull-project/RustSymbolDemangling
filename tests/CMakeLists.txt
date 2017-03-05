set(rsd_tests_sources
  RustSymbolDemanglingTest.cpp
)

add_executable(RSDTests ${rsd_tests_sources})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${RSD_SOURCE_DIR})

target_link_libraries(RSDTests
  RustSymbolDemangling

  ${RSD_DEPENDENCY_GOOGLETEST}
  ${RSD_DEPENDENCY_GOOGLETESTMAIN}
)
