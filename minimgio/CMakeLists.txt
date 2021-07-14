project(minimgio)

option(MINIMGIO_GENERATE "Include code that produces binary streams from MinImg" ON)
option(MINIMGIO_WRITE "Allow writing data to devices" ON)
option(WITH_DSHOW "Use DirectX Show" OFF)
if (MINIMGIO_GENERATE)
  add_definitions(-DMINIMGIO_GENERATE)
endif()
if (MINIMGIO_WRITE)
  add_definitions(-DMINIMGIO_WRITE)
endif()

set(STATUS_PNG "OFF")
set(STATUS_JPEG "OFF")
set(STATUS_TIFF "OFF")
set(STATUS_WEBP "OFF")

set(minimgio_public_HEADERS
  include/minimgio/define.h
  include/minimgio/device.h
  include/minimgio/contrib.h
  include/minimgio/minimgio.h
  include/minimgio/minimgio.hpp
  include/minimgio/minimgio_types.h
)

set(minimgio_HEADERS
  ${minimgio_public_HEADERS}
  src/minimgiodevice.h
  src/subsystem.h
  src/camstream.h
  src/stream.h
  src/utils.h
  src/pack.h)

set(minimgio_SRCS
  src/minimgio.cpp
  src/minimgio_ancient.cpp
  src/binary_stream.cpp
  src/device.cpp
  src/contrib.cpp
  src/minimgiolst.cpp
  src/minimgiodevice.cpp
  src/subsystem.cpp
  src/camstream.cpp
  src/utils.cpp
  src/pack.cpp)

set(thirdparty_LIBS)

option(WITH_JPEG "Turns on support for JPEG image format." ON)
option(WITH_SYSTEM_JPEG "Use system JPEG library." OFF)
if (WITH_JPEG)
  add_definitions(-DWITH_JPEG)
  set(minimgio_SRCS ${minimgio_SRCS} src/minimgiojpeg.cpp)
  if (WITH_SYSTEM_JPEG)
    find_package(JPEG REQUIRED)
    set(STATUS_JPEG "SYSTEM")
    set(thirdparty_LIBS ${thirdparty_LIBS} ${JPEG_LIBRARIES})
  else()
    set(STATUS_JPEG "PROJECT")
    set(thirdparty_LIBS ${thirdparty_LIBS} jpeg)
  endif()
endif (WITH_JPEG)

option(WITH_TIFF "Turns on support for TIFF image format." ON)
option(WITH_SYSTEM_TIFF "Use system TIFF library." OFF)
if (WITH_TIFF)
  add_definitions(-DWITH_TIFF)
  set(minimgio_SRCS ${minimgio_SRCS} src/minimgiotiff.cpp)
  if (WITH_SYSTEM_TIFF)
    find_package(TIFF REQUIRED)
    set(STATUS_TIFF "SYSTEM")
    include_directories(${TIFF_INCLUDE_DIR})
    set(thirdparty_LIBS ${thirdparty_LIBS} ${TIFF_LIBRARIES})
  else()
    set(STATUS_TIFF "PROJECT")
    set(thirdparty_LIBS ${thirdparty_LIBS} tiff)
  endif()
endif (WITH_TIFF)

option(WITH_PNG "Turns on support for PNG image format." ON)
option(WITH_SYSTEM_PNG "Use system PNG library." OFF)
if (WITH_PNG)
  add_definitions(-DWITH_PNG)
  set(minimgio_SRCS ${minimgio_SRCS} src/minimgiopng.cpp)
  if (WITH_SYSTEM_PNG)
    find_package(PNG REQUIRED)
    set(STATUS_PNG "SYSTEM")
    include_directories(${PNG_INCLUDE_DIRS})
    set(thirdparty_LIBS ${thirdparty_LIBS} ${PNG_LIBRARIES})
  else()
    set(STATUS_PNG "PROJECT")
    set(thirdparty_LIBS ${thirdparty_LIBS} png)
  endif()
endif (WITH_PNG)

if (WITH_TIFF OR WITH_PNG)
  # low quality code below is required because someone removed `zlib` linkage
  # in `libpng` and possibly in `libtiff` library too
  set(thirdparty_LIBS ${thirdparty_LIBS} zlib)
endif()

option(WITH_WEBP "Turns on support for WebP image format." OFF)
option(WITH_SYSTEM_WEBP "Turns on support for WebP image format." OFF)
if (WITH_WEBP)
  add_definitions(-DWITH_WEBP)
  set(minimgio_SRCS ${minimgio_SRCS} src/minimgiowebp.cpp)
  if (WITH_SYSTEM_WEBP)
    find_library(WEBP_LIBRARY webp libwebp)
    set(STATUS_WEBP "SYSTEM")
    if (NOT WEBP_LIBRARY)
      message(FATAL_ERROR "WebP not found. Disable WITH_SYSTEM_WEBP.")
    endif()
    set(thirdparty_LIBS ${thirdparty_LIBS} ${WEBP_LIBRARY})
  else()
    set(STATUS_WEBP "PROJECT")
    set(thirdparty_LIBS ${thirdparty_LIBS} libwebp)
  endif()
endif (WITH_WEBP)

option(WITH_DSHOW "Turns on dshow support for video input." OFF)
option(WITH_V4L2 "Turns on v4l2 support for video input." OFF)
option(WITH_AVFOUNDATION "Turns on avfoundation support for video input." OFF)

if(WITH_DSHOW AND MSVC)

  add_definitions(-DDIRECTX_SHOW)

  set(minimgio_HEADERS
    ${minimgio_HEADERS}
    src/dshowutils.h
    src/dshowdevice.h
    src/dshowsubsystem.h)

  set(minimgio_SRCS
    ${minimgio_SRCS}
    src/dshowutils.cpp
    src/dshowdevice.cpp
    src/dshowsubsystem.cpp)

  set(thirdparty_LIBS
    ${thirdparty_LIBS}
    winmm
    strmiids)

elseif(WITH_V4L2)

  add_definitions(-DV4L2)

  set(v4l2_HEADERS
    src/v4l2device.h
    src/v4l2subsystem.h
    src/v4l2utils.h)
  set(minimgio_HEADERS
    ${minimgio_HEADERS}
    ${v4l2_HEADERS})

  set(v4l2_SRCS
    src/v4l2device.cpp
    src/v4l2subsystem.cpp
    src/v4l2utils.cpp)
  set(minimgio_SRCS
    ${minimgio_SRCS}
    ${v4l2_SRCS})

  set(thirdparty_LIBS
    ${thirdparty_LIBS}
    v4l2)

elseif(WITH_AVFOUNDATION)

  add_definitions(-DAVFOUNDATION)

  set(av_HEADERS
    src/avfoundationdevice.h
    src/avfoundationsubsystem.h
    src/avfoundationutils.h)
  set(minimgio_HEADERS
    ${minimgio_HEADERS}
    ${av_HEADERS})

  set(av_SRCS
    src/avfoundationdevice.mm
    src/avfoundationsubsystem.mm)
  set(minimgio_SRCS
    ${minimgio_SRCS}
    ${av_SRCS})

endif()

if(BUILD_SHARED_LIBS)
  add_definitions(-DMINIMGIO_EXPORTS)
endif()

add_library(minimgio ${minimgio_SRCS} ${minimgio_HEADERS})
target_include_directories(minimgio PUBLIC include)

# target_link_libraries can't take empty argument - so we check
if(thirdparty_LIBS)
   target_link_libraries(minimgio ${thirdparty_LIBS})
endif()
target_link_libraries(minimgio minimgapi)

if (TARGET benchmark)  # google benchmark
  add_subdirectory(benchmark)
endif (TARGET benchmark)

if(BUILD_GO)
  add_executable(minimgiodev_go src/minimgiodev_go.cpp)
  target_link_libraries(minimgiodev_go minimgio minimgapi)

  if(WITH_AVFOUNDATION)
    macro(ADD_FRAMEWORK appname fwname)
    find_library(FRAMEWORK_${fwname}
        NAMES ${fwname}
        PATHS ${CMAKE_OSX_SYSROOT}/System/Library
        PATH_SUFFIXES Frameworks
        NO_DEFAULT_PATH)
    if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
        MESSAGE(ERROR ": Framework ${fwname} not found")
    else()
        TARGET_LINK_LIBRARIES(${appname} "${FRAMEWORK_${fwname}}/${fwname}")
        MESSAGE(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
    endif()
    endmacro(ADD_FRAMEWORK)

    # SET (CMAKE_EXE_LINKER_FLAGS
    # "-framework CoreMedia -framework CoreVideo -framework AVFoundation -framework Foundation")

    add_framework(minimgiodev_go CoreMedia)
    add_framework(minimgiodev_go CoreVideo)
    add_framework(minimgiodev_go Foundation)
    add_framework(minimgiodev_go AVFoundation)
   # add_framework(minimgiodev_go CoreGraphics)
   # add_framework(minimgiodev_go MobileCoreServices)
   # add_framework(minimgiodev_go ImageIO)

  endif(WITH_AVFOUNDATION)
endif(BUILD_GO)

if(MINIMGIO_GENERATE AND WITH_TESTS)
 add_subdirectory(test)
endif(MINIMGIO_GENERATE AND WITH_TESTS)

message(STATUS "${PROJECT_NAME}: PNG[${STATUS_PNG}] JPEG[${STATUS_JPEG}] "
               "TIFF[${STATUS_TIFF}] WEBP[${STATUS_WEBP}] LST[INTERNAL]")

install(FILES ${minimgio_public_HEADERS}
  DESTINATION include/minimgio)
install(TARGETS minimgio
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
