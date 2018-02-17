project (MinGfxExample)

include(${CMAKE_FRAMEWORK_DIR}/project/project.cmake)

# --------------------- Source -----------------------

set (SOURCEFILES
  src/gui_plus_opengl.cc
  src/main.cc
)

set (HEADERFILES
  src/gui_plus_opengl.h
)

# --------------------- Dependencies -----------------------

set(MinGfxArgs -DAUTOBUILD_NANOGUI=ON)
add_external(mingfx 
	GIT_REPOSITORY https://github.com/ivlab/MinGfx.git
	GIT_TAG master
	CMAKE_ARGS MinGfxArgs
	LIB_NAME MinGfx
)

include_directories(
	${external_dir}/mingfx/build/install/include/MinGfx-1.0
	${external_dir}/mingfx/build/install/include/nanovg
)

link_directories(
	${external_dir}/mingfx/build/install/lib/MinGfx-1.0
	${external_dir}/mingfx/build/install/lib
)

include(${external_dir}/CPP11/CPP11.cmake)
useCPP11()

include(${external_dir}/OpenGL/OpenGL.cmake)
useOpenGL()

# --------------------- Executable -----------------------

addStandardExecutable()

target_link_libraries(${PROJECT_NAME} nanogui)