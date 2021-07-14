project(minutils)

if(WITH_TESTS)
  add_definitions(-DUNIT_TEST)
endif(WITH_TESTS)

FILE(GLOB MINUTILS_PUBLIC_HEADERS
  include/minutils/minhelpers.h
  include/minutils/smartptr.h
  include/minutils/mathoper.h
)

set(MINUTILS_INTERNAL_HEADERS)

FILE(GLOB MINUTILS_SOURCES
  src/minutils.cpp)

add_definitions(-DMINUTILS_EXPORTS)
add_library(minutils STATIC
            ${MINUTILS_SOURCES} ${MINUTILS_INTERNAL_HEADERS} ${MINUTILS_PUBLIC_HEADERS})
target_include_directories(minutils PUBLIC include)

# Add logging support on Android
if (ANDROID)
  target_link_libraries(minutils log)
endif()

install(DIRECTORY include/ DESTINATION include)
install(TARGETS minutils
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
