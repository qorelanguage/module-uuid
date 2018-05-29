cmake_minimum_required(VERSION 2.8.3)

project(qore-uuid-module)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 4)
set (VERSION_PATCH 0)


find_package(Qore 0.9 REQUIRED)

include_directories( ${CMAKE_SOURCE_DIR}/src )

# Check for C++11.
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

find_path(UUID_INCLUDE_DIR uuid.h PATH_SUFFIXES uuid ossp)

include(CheckFunctionExists)
# Some systems include uuid automatically (OS X), others need the includes/library
check_function_exists (uuid_unparse_lower HAVE_UUID)
if (NOT HAVE_UUID)
    message ("-- Looking for libuuid")
    find_library(UUID_LIBRARY NAMES uuid)
    if (UUID_INCLUDE_DIR AND UUID_LIBRARY)
        message ("-- Found libuuid: ${UUID_LIBRARY}")
        set (HAVE_UUID true)
    else (UUID_INCLUDE_DIR AND UUID_LIBRARY)
        message(FATAL_ERROR "no libuuid found")
    endif (UUID_INCLUDE_DIR AND UUID_LIBRARY)
endif (NOT HAVE_UUID)

include_directories(${UUID_INCLUDE_DIR})

set(QPP_SRC
    src/QC_UUID.qpp
)

set(CPP_SRC
    src/uuid-module.cpp
)

set(QMOD
)

qore_wrap_qpp_value(QPP_SOURCES ${QPP_SRC})

SET (module_name "uuid")

add_library(${module_name} SHARED ${QPP_SOURCES} ${CPP_SRC})

qore_binary_module(${module_name} "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" ${UUID_LIBRARY})
qore_user_modules("${QMOD}")

qore_dist("${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

qore_config_info()


