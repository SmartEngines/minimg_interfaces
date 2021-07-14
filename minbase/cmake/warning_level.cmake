function(min_target_set_warning_level target level)
  # known issue: https://gitlab.kitware.com/cmake/cmake/-/issues/19084
  # gcc options: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
  # msvc options: https://docs.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level
  if(level STREQUAL "suppress")
    if (MSVC)
      target_compile_options(${target} PRIVATE /w)
    else()
      target_compile_options(${target} PRIVATE -w)
    endif()
  elseif(level STREQUAL "normal")
    if (MSVC)
      target_compile_options(${target} PRIVATE /W2)
    else()
      target_compile_options(${target} PRIVATE -Wall)
    endif()
  elseif(level STREQUAL "extra")
    if (MSVC)
      target_compile_options(${target} PRIVATE /W3)
    else()
      target_compile_options(${target} PRIVATE -Wall -Wextra)
    endif()
  elseif(level STREQUAL "ultra")
    if (MSVC)
      target_compile_options(${target} PRIVATE /W4)
    else()
      target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
  else()
    message(FATAL_ERROR "Invalid level `${level}`")
  endif()
endfunction()

function(min_target_supress_warnings target)
  # this code exists because `target_compile_options` can't set flags for
  # external projects
  if(MSVC)
    set(marning_flag "/w")
  else()
    set(marning_flag "-w")
  endif()
  set_property(TARGET ${target} APPEND_STRING PROPERTY COMPILE_FLAGS ${marning_flag})
endfunction()