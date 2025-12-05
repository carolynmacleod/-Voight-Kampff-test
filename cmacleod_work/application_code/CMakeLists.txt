cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(vtk_test_project)

find_package(VTK COMPONENTS 
  CommonColor
  CommonCore
  FiltersSources
  InteractionStyle
  RenderingContextOpenGL2
  RenderingCore
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingOpenGL2
)

if (NOT VTK_FOUND)
  message(FATAL_ERROR "vtk_test_project: Unable to find the VTK build folder.")
endif()

# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
add_executable(vtk_test_project main.cpp inputLoader.cpp VisualizeEvent.cpp )
  target_link_libraries(vtk_test_project PRIVATE ${VTK_LIBRARIES}
)
# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS vtk_test_project
  MODULES ${VTK_LIBRARIES}
)