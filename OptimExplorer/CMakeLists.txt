cmake_minimum_required(VERSION 3.10)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

# The following code block is suggested to be used on Windows. According to
# https://github.com/pytorch/pytorch/issues/25457, the DLLs need to be copied to
# avoid memory errors.
if(MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(
    TARGET TorchExample
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TORCH_DLLS}
            $<TARGET_FILE_DIR:TorchExample>)
endif(MSVC)

option(MAHI_UTIL_DEFAULT_LOG
       "Turn ON to enable a default log output to console/file" ON)
set(MAHI_UTIL_DEFAULT_LOG OFF)
add_subdirectory(vendor/mahi)

add_executable(
  OptimExplorer main.cpp core/app.cpp core/events.cpp torch/torch.cpp widgets/metrics.cpp
                widgets/train.cpp widgets/utils.cpp)

set(TARGET_COMPILE_DEFINITIONS "")
set(TARGET_PRIVATE_LINK_LIBS mahi::gui "${TORCH_LIBRARIES}")

if(WIN32)
  list(APPEND TARGET_COMPILE_DEFINITIONS "PLATFORM_WINDOWS")
elseif(UNIX)
  list(APPEND TARGET_COMPILE_DEFINITIONS "PLATFORM_LINUX")
endif()
target_compile_definitions(OptimExplorer PRIVATE ${TARGET_COMPILE_DEFINITIONS})

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  list(APPEND TARGET_COMPILE_DEFINITIONS "DEBUG" "PROFILING")
endif()

if(MSVC)
  target_compile_options(OptimExplorer PRIVATE "/W4")
else()
  if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    target_compile_options(
      OptimExplorer
      PRIVATE "-O1"
              "-Wextra"
              "-Wpedantic"
              "-Wshadow"
              "-Wnon-virtual-dtor"
              "-Wnull-dereference"
              "-Wold-style-cast"
              "-Wuseless-cast"
              "-Wformat=2"
              "-fsanitize=address"
              "-Wreorder")
    list(APPEND TARGET_PRIVATE_LINK_LIBS -fsanitize=address)
  else()
    target_compile_options(OptimExplorer PRIVATE "-O3")
  endif()
endif()

target_include_directories(
  OptimExplorer
  PRIVATE "${PROJECT_SOURCE_DIR}/OptimExplorer/"
          "${PROJECT_SOURCE_DIR}/OptimExplorer/vendor/"
          "${PROJECT_SOURCE_DIR}/OptimExplorer/vendor/eventpp/include"
          "${TORCH_INSTALL_PREFIX}/"
          "${TORCH_INSTALL_PREFIX}/include/"
          "${TORCH_INSTALL_PREFIX}/include/torch/"
          "${TORCH_INSTALL_PREFIX}/include/torch/csrc/"
  PUBLIC "${PROJECT_SOURCE_DIR}/" "${PROJECT_BINARY_DIR}/"
  INTERFACE "${PROJECT_SOURCE_DIR}/OptimExplorer")

target_link_libraries(OptimExplorer PRIVATE ${TARGET_PRIVATE_LINK_LIBS})
