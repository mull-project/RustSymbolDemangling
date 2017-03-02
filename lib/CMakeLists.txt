set(rustdemangle_source_files
  rustdemangle.cpp
)

file(GLOB_RECURSE rustdemangle_header_files ${RUSTDEMANGLE_SOURCE_DIR}/*.h)

add_library(rustdemangle SHARED ${rustdemangle_source_files} ${rustdemangle_header_files})

set_source_files_properties(${rustdemangle_header_dirs} PROPERTIES HEADER_FILE_ONLY ON)

include_directories(${rustdemangle_header_dirs})

# The following is needed to make CMake to generate a folder structure in Xcode
# that corresponds to a physical folder structure.
get_target_property(HERE_TARGET_SOURCES rustdemangle SOURCES)
foreach(source_file ${HERE_TARGET_SOURCES})
  if (source_file MATCHES "(.*)\.h$")
    file(RELATIVE_PATH relative_source_file ${RUSTDEMANGLE_SOURCE_DIR} ${source_file})
    get_filename_component(relative_path ${relative_source_file} DIRECTORY)
    source_group("Header Files\\${relative_path}" FILES ${source_file})
  else()
    get_filename_component(relative_path ${source_file} DIRECTORY)
    source_group("Source Files\\${relative_path}" FILES ${source_file})
  endif()
endforeach()
