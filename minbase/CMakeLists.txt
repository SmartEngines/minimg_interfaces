cmake_minimum_required(VERSION 3.1.0)
project(minbase)

if(COMMAND package_version)
  package_version(minbase 0.1.0)
endif()


add_library(minbase
  # sources
  src/mintyp.cpp
  # headers
  include/minbase/minpreprocessor.h
  include/minbase/crossplat.h
  include/minbase/warnings.h
  include/minbase/mintyp.h
  include/minbase/minimg.h
  include/minbase/mingeo.h
  include/minbase/minstd.hpp
  include/minbase/introspection.hpp
  include/minbase/minmeta.hpp
  include/minbase/min_pp_mintyp.h
  include/minbase/meta_mintyp.hpp
  include/minbase/magic_switch.hpp
  include/minbase/minresult.h
  include/minbase/half.hpp

  include/minbase/gcc_versions_warnings/gcc_warnings.h
)
target_include_directories(minbase PUBLIC include)

if (BUILD_SHARED_LIBS)
  target_compile_definitions(minbase PUBLIC -DMINBASE_EXPORTS)
endif()

if (COMMAND target_enforce_warning_policy)
  target_enforce_warning_policy(minbase)
endif()


install(DIRECTORY include/minbase DESTINATION include)
install(TARGETS minbase
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)


## FIXME: Rework tests options
#if (WITH_TESTS)
#if (GLOBAL_TESTS_MODE EQUAL NO_TESTS)
#  set(WITH_TESTS_MINBASE 0 CACHE BOOL "Turns on minbase tests" FORCE)
#else ()
#  set(WITH_TESTS_MINBASE 1 CACHE BOOL "Turns on minbase tests" FORCE)
#endif ()

## For tags:
#if (GLOBAL_TESTS_MODE EQUAL FORCE_TESTS)
#  set(WITH_TESTS_MINBASE 1 CACHE BOOL "Turns on minbase tests" FORCE)
#else ()
#  set(WITH_TESTS_MINBASE 0 CACHE BOOL "Turns on minbase tests" FORCE)
#endif ()

if (WITH_TESTS)
  add_subdirectory(test)
endif(WITH_TESTS)

if (TARGET benchmark)  # google benchmark
  add_subdirectory(benchmark)
endif (TARGET benchmark)


### beautify in-IDE representation
if(NOT ${CMAKE_VERSION} VERSION_LESS 3.8)
  get_target_property(MINBASE_SOURCES minbase SOURCES)
  list(FILTER MINBASE_SOURCES INCLUDE REGEX "\.(h|hpp)^")
  source_group(
    TREE "${CMAKE_CURRENT_LIST_DIR}/include/minbase"
    PREFIX "Header Files"
    FILES ${MINBASE_SOURCES}
  )
  get_target_property(MINBASE_SOURCES minbase SOURCES)
  list(FILTER MINBASE_SOURCES INCLUDE REGEX "\.(c|cpp)^")
  source_group(
    TREE "${CMAKE_CURRENT_LIST_DIR}/src"
    PREFIX "Source Files"
    FILES ${MINBASE_SOURCES}
  )
endif()
