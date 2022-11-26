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

add_executable(vbfeditor main.cpp VbfFile.cpp VbfFile.h)
if(WIN32)
    target_link_libraries(vbfeditor ws2_32)
endif()

install(TARGETS vbfeditor)