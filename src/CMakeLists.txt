set(EXECUTABLE_NAME ${PROJECT_NAME})

if(MSVC)
    string(REGEX REPLACE "/W[1-3]" "/W4" "/O1" "/O2" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror -O3)
endif()

file(GLOB_RECURSE cpp_files "./*.cpp")
file(GLOB_RECURSE h_files "./*.h")
file(GLOB_RECURSE hpp_files "./*.hpp")

add_executable(${EXECUTABLE_NAME}
    ${cpp_files}
    ${h_files}
    ${hpp_files}

    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui.h
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_internal.h
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp

    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp

    ${implot_SOURCE_DIR}/implot.cpp
    ${implot_SOURCE_DIR}/implot.h
    ${implot_SOURCE_DIR}/implot_demo.cpp
    ${implot_SOURCE_DIR}/implot_internal.h
    ${implot_SOURCE_DIR}/implot_items.cpp
) 

target_compile_definitions(${EXECUTABLE_NAME}
    PRIVATE
    GLEW_STATIC
)

target_include_directories(${EXECUTABLE_NAME}
    PRIVATE
    ${imgui_SOURCE_DIR}/
)

target_include_directories(${EXECUTABLE_NAME}
    PRIVATE
    ${implot_SOURCE_DIR}/
)

if(WIN32)
    target_link_libraries(${EXECUTABLE_NAME}
        PRIVATE
        glfw
        libglew_static
        spdlog
        $<$<CXX_COMPILER_ID:GNU>:opengl32.a>
        $<$<CXX_COMPILER_ID:MSVC>:opengl32.lib>
    )
else()
    target_link_libraries(${EXECUTABLE_NAME}
        PRIVATE
        glfw
        libglew_static
        spdlog
    )
endif()