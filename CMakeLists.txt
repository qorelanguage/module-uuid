cmake_minimum_required(VERSION 2.8.3)

project(qore-uuid-module)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 4)
set (VERSION_PATCH 0)


find_package(Qore REQUIRED)

include_directories( ${CMAKE_SOURCE_DIR}/src )

include(CheckFunctionExists)
# Some systems include uuid automatically (OS X), others need the includes/library
check_function_exists (uuid_unparse_lower HAVE_UUID)
if (NOT HAVE_UUID)
    message ("-- Looking for libuuid")
    find_path(UUID_INCLUDE_DIR uuid/uuid.h)
    find_library(UUID_LIBRARY NAMES uuid)
    if (UUID_INCLUDE_DIR AND UUID_LIBRARY)
        message ("-- Found libuuid: ${UUID_LIBRARY}")
        set (HAVE_UUID true)
    else (UUID_INCLUDE_DIR AND UUID_LIBRARY)
        message(FATAL_ERROR "no libuuid found")
    endif (UUID_INCLUDE_DIR AND UUID_LIBRARY)
endif (NOT HAVE_UUID)

include_directories(${UUID_INCLUDE_DIR}/uuid)

set(QPP_SRC
    src/QC_UUID.qpp
)

set(CPP_SRC
    src/uuid-module.cpp
)

set(QMOD
)

qore_wrap_qpp(QPP_SOURCES ${QPP_SRC})

SET (module_name "uuid")

add_library(${module_name} SHARED ${QPP_SOURCES} ${CPP_SRC})

qore_binary_module(${module_name} "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" ${UUID_LIBRARY})
qore_user_modules("${QMOD}")

qore_dist("${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

qore_config_info()


