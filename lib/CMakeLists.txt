set(rsd_source_files
  RustSymbolDemangling.cpp
)

file(GLOB_RECURSE rsd_header_files ${RSD_SOURCE_DIR}/*.h)

add_library(RustSymbolDemangling SHARED ${rsd_source_files} ${rsd_header_files})

set_source_files_properties(${rsd_header_dirs} PROPERTIES HEADER_FILE_ONLY ON)

include_directories(${rsd_header_dirs})

# The following is needed to make CMake to generate a folder structure in Xcode
# that corresponds to a physical folder structure.
get_target_property(HERE_TARGET_SOURCES RustSymbolDemangling SOURCES)
foreach(source_file ${HERE_TARGET_SOURCES})
  if (source_file MATCHES "(.*)\.h$")
    file(RELATIVE_PATH relative_source_file ${RSD_SOURCE_DIR} ${source_file})
    get_filename_component(relative_path ${relative_source_file} DIRECTORY)
    source_group("Header Files\\${relative_path}" FILES ${source_file})
  else()
    get_filename_component(relative_path ${source_file} DIRECTORY)
    source_group("Source Files\\${relative_path}" FILES ${source_file})
  endif()
endforeach()
