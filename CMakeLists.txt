cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/dep/glfw-3.3.8.bin.WIN64/include
	${CMAKE_CURRENT_SOURCE_DIR}/dep/glew-2.1.0/include
	${CMAKE_CURRENT_SOURCE_DIR}/dep/glm
	${CMAKE_CURRENT_SOURCE_DIR}/dep/imgui
	${CMAKE_CURRENT_SOURCE_DIR}/dep/happly
)

link_libraries(
    ${CMAKE_CURRENT_SOURCE_DIR}/dep/glfw-3.3.8.bin.WIN64/lib-vc2022/glfw3.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/dep/glfw-3.3.8.bin.WIN64/lib-vc2022/glfw3_mt.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/dep/glew-2.1.0/lib/Release/x64/glew32s.lib
    OpenGL32.lib
)

#################################################
add_executable(load_ply #WIN32
    # opengl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/GLDebug.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/glviz/buffer.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/glviz/camera.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/glviz/program.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/glviz/shader.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/glviz/trackball.cpp

    # ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/framebuffer.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/program_attribute.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/program_finalization.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/splat_renderer.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/shaders/attribute_fs_glsl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/shaders/attribute_vs_glsl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/shaders/finalization_fs_glsl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/shaders/finalization_vs_glsl.cpp
    # imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/dep/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dep/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dep/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dep/imgui/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dep/imgui/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dep/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dep/imgui/imgui_widgets.cpp
    # main
    ${CMAKE_CURRENT_SOURCE_DIR}/src/App.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

set_target_properties(load_ply PROPERTIES LINK_OPTIONS "/NODEFAULTLIB:LIBCMT")
#################################################