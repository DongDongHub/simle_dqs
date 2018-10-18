#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "json11" for configuration ""
set_property(TARGET json11 APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(json11 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib64/libjson11.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS json11 )
list(APPEND _IMPORT_CHECK_FILES_FOR_json11 "${_IMPORT_PREFIX}/lib64/libjson11.a" )

# Import target "ppconsul" for configuration ""
set_property(TARGET ppconsul APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(ppconsul PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C;CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib64/libppconsul.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS ppconsul )
list(APPEND _IMPORT_CHECK_FILES_FOR_ppconsul "${_IMPORT_PREFIX}/lib64/libppconsul.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
