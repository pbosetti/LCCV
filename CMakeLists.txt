#   _     ____ ______     __
#  | |   / ___/ ___\ \   / /
#  | |  | |  | |    \ \ / / 
#  | |__| |__| |___  \ V /  
#  |_____\____\____|  \_/   
#                          
cmake_minimum_required(VERSION 3.6)

project(lccv VERSION 0.1.0 LANGUAGES CXX)

if(NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
    message(STATUS "No previous build - default to Release build")
  endif()
endif()

option(BUILD_EXAMPLES "Build example programs" OFF)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 17)
add_compile_options(
  -Wall -Wextra -pedantic -Wno-unused-parameter 
  -faligned-new -Werror -Wfatal-errors
)
add_definitions(-D_FILE_OFFSET_BITS=64)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wno-psabi)
endif()

if(NOT ENABLE_COMPILE_FLAGS_FOR_TARGET)
  # On a Pi this will give us armhf or arm64.
  execute_process(COMMAND dpkg-architecture -qDEB_HOST_ARCH
    OUTPUT_VARIABLE ENABLE_COMPILE_FLAGS_FOR_TARGET
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
endif()

message(STATUS "Platform: ${ENABLE_COMPILE_FLAGS_FOR_TARGET}")
if ("${ENABLE_COMPILE_FLAGS_FOR_TARGET}" STREQUAL "arm64")
  # 64-bit binaries can be fully optimised.
  add_definitions(-ftree-vectorize)
elseif ("${ENABLE_COMPILE_FLAGS_FOR_TARGET}" STREQUAL "armv8-neon")
  # Only build with 32-bit Pi 3/4 specific optimisations if requested on the 
  # command line.
  add_definitions(-mfpu=neon-fp-armv8 -ftree-vectorize)
endif()

find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCAMERA REQUIRED libcamera)
message(STATUS "libcamera library found:")
message(STATUS "    version: ${LIBCAMERA_VERSION}")
message(STATUS "    libraries: ${LIBCAMERA_LINK_LIBRARIES}")
message(STATUS "    include path: ${LIBCAMERA_INCLUDE_DIRS}")

set(SOURCES
  src/lccv.cpp
  src/libcamera_app.cpp
  src/libcamera_app_options.cpp
)
set(HEADERS
  include/lccv.hpp
  include/libcamera_app.hpp
  include/libcamera_app_options.hpp
)

include_directories(include ${LIBCAMERA_INCLUDE_DIRS} ${OPENCV_INCLUDE_DIRS})


#   _____  _    ____   ____ _____ _____ ____  
#  |_   _|/ \  |  _ \ / ___| ____|_   _/ ___| 
#    | | / _ \ | |_) | |  _|  _|   | | \___ \ 
#    | |/ ___ \|  _ <| |_| | |___  | |  ___) |
#    |_/_/   \_\_| \_\\____|_____| |_| |____/ 
#                                            

add_library(lccv SHARED ${SOURCES})
target_link_libraries(lccv pthread ${LIBCAMERA_LINK_LIBRARIES} ${OpenCV_LIBS})
target_compile_options(lccv PUBLIC -fPIC)

add_library(lccv_static STATIC ${SOURCES})
set_target_properties(lccv_static PROPERTIES OUTPUT_NAME "lccv")
target_compile_options(lccv_static PUBLIC -fPIC)
target_link_libraries(lccv_static pthread 
  ${LIBCAMERA_LINK_LIBRARIES} ${OpenCV_LIBS}
)

if(${BUILD_EXAMPLES})
  message(STATUS "Also building examples")
  add_subdirectory(example)
endif()


#   ___ _   _ ____ _____  _    _     _     
#  |_ _| \ | / ___|_   _|/ \  | |   | |    
#   | ||  \| \___ \ | | / _ \ | |   | |    
#   | || |\  |___) || |/ ___ \| |___| |___ 
#  |___|_| \_|____/ |_/_/   \_\_____|_____|
#                                         
include(GNUInstallDirs)
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
install(TARGETS lccv lccv_static 
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} 
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(
  FILES ${HEADERS} 
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)


# Source package generation setup.
set(CPACK_GENERATOR "TXZ")
set(CPACK_PACKAGE_FILE_NAME "lccv-build")
set(CPACK_SOURCE_GENERATOR "TXZ")
set(CPACK_INSTALL_SCRIPTS ${CMAKE_SOURCE_DIR}/package.cmake)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "lccv-src")
set(CPACK_SOURCE_IGNORE_FILES "/\.git*;/build;")
include(CPack)