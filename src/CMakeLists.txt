add_library(dkvs_core INTERFACE)
add_library(dkvs::core ALIAS dkvs_core)

target_include_directories(dkvs_core INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

add_executable(dkvstore main.cpp)
target_link_libraries(dkvstore PRIVATE dkvs::core)
