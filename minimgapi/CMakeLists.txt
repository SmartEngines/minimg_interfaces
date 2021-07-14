project(minimgapi)


#
# minimg* options
#

option(MINIMG_LINK_TIME_SIZE_OPTIMIZATION "TODO: Description" OFF)


#
# minimgapi options
#

option(MINIMGAPI_STOPWATCH_NORMAL_INTERFACE "Use minstopwatches (if enabled) for normal minimgapi interface" OFF)
option(MINIMGAPI_STOPWATCH_RAW_INTERFACE "Use minstopwatches (if enabled) for raw minimgapi interface" OFF)
option(MINIMGAPI_STOPWATCH_OLD_INTERFACE "Use minstopwatches (if enabled) for old minimgapi interface" OFF)


#
# source code files
#

set(MINIMGAPI_PUBLIC_HEADERS
  # old interface
  include/minimgapi/minimgapi-inl.h
  include/minimgapi/minimgapi.h
  include/minimgapi/minimgapi-helpers.hpp
  include/minimgapi/imgguard.hpp

  # normal interface
  include/minimgapi/minimgapi_types.h
  include/minimgapi/minimgapi_base.hpp
  include/minimgapi/assures_and_compares.hpp
  include/minimgapi/gets.hpp
  include/minimgapi/allocation.hpp
  include/minimgapi/prototype.hpp
  include/minimgapi/wrap.hpp
  include/minimgapi/reshape.hpp
  include/minimgapi/minimgapi.hpp

  # raw interface
  include/minimgapi/raw/minimgapi_raw_base.hpp
  include/minimgapi/raw/get_coord.hpp
  include/minimgapi/raw/gets.hpp
  include/minimgapi/raw/assures_and_compares.hpp
  include/minimgapi/raw/allocation.hpp
  include/minimgapi/raw/prototype.hpp
  include/minimgapi/raw/wrap.hpp
  include/minimgapi/raw/reshape.hpp
  include/minimgapi/raw/minimgapi_raw.hpp
)

set(MINIMGAPI_SOURCES
  src/bitcpy.h
  src/bitcpy.cpp
  src/copy_channels.cpp
  src/minimgapi.cpp
  src/resample.cpp
  src/transpose.cpp
)

set(MINIMGAPI_VECTOR_HEADERS
  src/vector/copy_channels-inl.h
  src/vector/transpose-inl.h
)

set(MINIMGAPI_VECTOR_NEON_HEADERS
  src/vector/neon/copy_channels-inl.h
  src/vector/neon/transpose-inl.h
)

set(MINIMGAPI_VECTOR_SSE_HEADERS
  src/vector/sse/copy_channels-inl.h
  src/vector/sse/transpose-inl.h
)


#
# default link libraries and compile definitions
#

set(MINIMGAPI_PUBLIC_LINK_LIBRARIES
  minbase
  minutils
)
set(MINIMGAPI_PRIVATE_LINK_LIBRARIES)

set(MINIMGAPI_PUBLIC_COMPILE_DEFINITIONS)
set(MINIMGAPI_PRIVATE_COMPILE_DEFINITIONS)


#
# check options and modify link libraries and compile definitions lists as needed
#

if (BUILD_SHARED_LIBS)
  list(APPEND MINIMGAPI_PUBLIC_COMPILE_DEFINITIONS -DMINIMGAPI_EXPORTS)
endif()

if (USE_ELBRUS_SIMD)
  list(APPEND MINIMGAPI_PUBLIC_LINK_LIBRARIES eml)
endif()

if (MINIMG_LINK_TIME_SIZE_OPTIMIZATION)
  list(APPEND MINIMGAPI_PRIVATE_COMPILE_DEFINITIONS -DMINIMG_LINK_TIME_SIZE_OPTIMIZATION)
endif(MINIMG_LINK_TIME_SIZE_OPTIMIZATION)

if (WITH_TIMING)
  list(APPEND MINIMGAPI_PUBLIC_LINK_LIBRARIES minstopwatch)
  if (MINIMGAPI_STOPWATCH_NORMAL_INTERFACE)
    list(APPEND MINIMGAPI_PUBLIC_COMPILE_DEFINITIONS -DMINIMGAPI_STOPWATCH_NORMAL_INTERFACE)
  endif()
  if (MINIMGAPI_STOPWATCH_RAW_INTERFACE)
    list(APPEND MINIMGAPI_PUBLIC_COMPILE_DEFINITIONS -DMINIMGAPI_STOPWATCH_RAW_INTERFACE)
  endif()
  if (MINIMGAPI_STOPWATCH_OLD_INTERFACE)
    list(APPEND MINIMGAPI_PUBLIC_COMPILE_DEFINITIONS -DMINIMGAPI_STOPWATCH_OLD_INTERFACE)
  endif()
endif()

#
# check everything if needed
#

# TODO: Maybe better to check target props after target_link_libraries
#       to see all dependencies that were brought transitively

#message(STATUS "MINIMGAPI_PUBLIC_LINK_LIBRARIES: ${MINIMGAPI_PUBLIC_LINK_LIBRARIES}")
#message(STATUS "MINIMGAPI_PRIVATE_LINK_LIBRARIES: ${MINIMGAPI_PRIVATE_LINK_LIBRARIES}")
#message(STATUS "MINIMGAPI_PUBLIC_COMPILE_DEFINITIONS: ${MINIMGAPI_PUBLIC_COMPILE_DEFINITIONS}")
#message(STATUS "MINIMGAPI_PRIVATE_COMPILE_DEFINITIONS: ${MINIMGAPI_PRIVATE_COMPILE_DEFINITIONS}")


#
# create target minimgapi
#

add_library(minimgapi
  ${MINIMGAPI_SOURCES}
  ${MINIMGAPI_PUBLIC_HEADERS}
  ${MINIMGAPI_VECTOR_HEADERS}
  ${MINIMGAPI_VECTOR_NEON_HEADERS}
  ${MINIMGAPI_VECTOR_SSE_HEADERS}
)
target_include_directories(minimgapi PUBLIC include)

target_link_libraries(minimgapi
  PUBLIC  ${MINIMGAPI_PUBLIC_LINK_LIBRARIES}
  PRIVATE ${MINIMGAPI_PRIVATE_LINK_LIBRARIES})
target_compile_definitions(minimgapi
  PUBLIC  ${MINIMGAPI_PUBLIC_COMPILE_DEFINITIONS}
  PRIVATE ${MINIMGAPI_PRIVATE_COMPILE_DEFINITIONS})

if (COMMAND target_enforce_warning_policy)
#  target_enforce_warning_policy(minimgapi)
endif()


#
# install
#

install(DIRECTORY include/minimgapi DESTINATION include)
install(TARGETS minimgapi
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

#
# add tests
#

# TODO: Check if there is still any problem with android
if (WITH_TESTS) # AND NOT ANDROID)
  add_subdirectory(test)
endif()


#
# beautify in-IDE representation
#

if(NOT ${CMAKE_VERSION} VERSION_LESS 3.8)
  source_group(
    TREE "${CMAKE_CURRENT_LIST_DIR}/include/minimgapi"
    PREFIX "Header Files"
    FILES ${MINIMGAPI_PUBLIC_HEADERS})
  source_group(
    TREE "${CMAKE_CURRENT_LIST_DIR}/src"
    PREFIX "Source Files"
    FILES
      ${MINIMGAPI_SOURCES}
      ${MINIMGAPI_VECTOR_HEADERS}
      ${MINIMGAPI_VECTOR_NEON_HEADERS}
      ${MINIMGAPI_VECTOR_SSE_HEADERS})
endif()
