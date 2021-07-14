# Use to organize targets in an IDE.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# this MUST be a `macro`, because a `function` would eat set(..., PARENT_SCOPE)
macro(add_subdirectory name)
  set(ORIGIANL_CMAKE_FOLDER ${CMAKE_FOLDER})
  if (DEFINED CMAKE_FOLDER)
    set(CMAKE_FOLDER ${CMAKE_FOLDER}/${name})
  else()
    set(CMAKE_FOLDER ${name})
  endif()
  _add_subdirectory(${name} ${ARGN})
  set(CMAKE_FOLDER ${ORIGIANL_CMAKE_FOLDER})
endmacro()
