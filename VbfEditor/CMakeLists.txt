# The version number.
set (vbfedit_VERSION_MAJOR 0)
set (vbfedit_VERSION_MINOR 1)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
        "VbfeditConfig.h.in"
        "${PROJECT_BINARY_DIR}/VbfeditConfig.h"
)
include_directories("${PROJECT_BINARY_DIR}")

include_directories(../rapidjson/include)
include_directories(../cxxopts/include)
include_directories(../CRCpp/inc)
include_directories(../lzss)

add_library(lzss ../lzss/lzss.c ../lzss/brute.c)
add_library(bitfile ../lzss/bitfile/bitfile.c)
add_library(vbf VbfFile.cpp VbfFile.h)
target_link_libraries(vbf lzss bitfile)
if(WIN32)
    target_link_libraries(vbf ws2_32)
endif()

add_executable(vbfeditor main.cpp)
target_link_libraries(vbfeditor vbf)

install(TARGETS vbfeditor)