# interface
add_executable(test_minimgio_interface test_interface.cpp)
target_link_libraries(test_minimgio_interface minimgio minimgapi gtest)
add_test(
  NAME test_minimgio_interface
  COMMAND test_minimgio_interface)

# png
if(WITH_PNG)  # since WITH_PNG=No is tested in test_interface
  add_executable(test_minimgio_png test_png.cpp)
  target_link_libraries(test_minimgio_png minimgio minimgapi gtest)
  add_test(
    NAME test_minimgio_png
    COMMAND test_minimgio_png)
endif(WITH_PNG)

# tiff
if(WITH_TIFF)  # since WITH_TIFF=No is tested in test_interface
  add_executable(test_minimgio_tiff test_tiff.cpp)
  target_link_libraries(test_minimgio_tiff minimgio minimgapi gtest)
  add_test(
    NAME test_minimgio_tiff
    COMMAND test_minimgio_tiff "${CMAKE_CURRENT_LIST_DIR}/images/")
  if (WITH_TIFF_LZW)
    target_compile_definitions(test_minimgio_tiff PRIVATE -DWITH_TIFF_LZW)
  endif()
  if (WITH_TIFF_ZIP)
    target_compile_definitions(test_minimgio_tiff PRIVATE -DWITH_TIFF_ZIP)
  endif()
  if (WITH_TIFF_JPEG)
    target_compile_definitions(test_minimgio_tiff PRIVATE -DWITH_TIFF_JPEG)
  endif()
endif(WITH_TIFF)

# jpeg
if(WITH_JPEG)  # since WITH_JPEG=No is tested in test_interface
  add_executable(test_minimgio_jpeg test_jpeg.cpp)
  target_link_libraries(test_minimgio_jpeg minimgio minimgapi gtest)
  add_test(
    NAME test_minimgio_jpeg
    COMMAND test_minimgio_jpeg "${CMAKE_CURRENT_LIST_DIR}/images/")
endif(WITH_JPEG)

# webp
if(WITH_WEBP)
  add_executable(test_minimgio_webp test_webp.cpp)
  target_link_libraries(test_minimgio_webp minimgio minimgapi gtest)
  add_test(
    NAME test_minimgio_webp
    COMMAND test_minimgio_webp)
endif()
