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

include_directories(lib)
add_library(vbf lib/vbf.cpp)
if(WIN32)
    target_link_libraries(vbf ws2_32)
endif()

add_executable(vbfeditor vbfeditor.cpp)
target_link_libraries(vbfeditor vbf)

install(TARGETS vbfeditor)