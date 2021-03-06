set(MIN_OUTPUT_ROOT_DIR "${CMAKE_BINARY_DIR}" CACHE PATH "Path where runtime output goes")

set(MIN_DEFAULT_PLATFORM)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  set(MIN_CMAKE_CXX_COMPILER_ID GCC)
else()
  set(MIN_CMAKE_CXX_COMPILER_ID ${CMAKE_CXX_COMPILER_ID})
endif()
string(TOLOWER
"${CMAKE_SYSTEM_PROCESSOR}\
-${CMAKE_SYSTEM_NAME}\
-${MIN_CMAKE_CXX_COMPILER_ID}\
-${CMAKE_CXX_COMPILER_VERSION}"
MIN_DEFAULT_PLATFORM
)

macro(min_set_output_dirs)
  if (${ARGC} EQUAL 1)
    set(MIN_PLATFORM ${ARGV0})
  else()
    set(MIN_PLATFORM ${MIN_DEFAULT_PLATFORM})
  endif()
  message(STATUS "minbase: Platform: ${MIN_PLATFORM}")
  message(STATUS "minbase: Output path: ${MIN_OUTPUT_ROOT_DIR}")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MIN_OUTPUT_ROOT_DIR}/bin.${MIN_PLATFORM}.$<LOWER_CASE:$<CONFIG>>)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MIN_OUTPUT_ROOT_DIR}/lib.${MIN_PLATFORM}.$<LOWER_CASE:$<CONFIG>>)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MIN_OUTPUT_ROOT_DIR}/lib.${MIN_PLATFORM}.$<LOWER_CASE:$<CONFIG>>)
endmacro()
