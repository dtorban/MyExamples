project (MinVRBetaExample)

include(${CMAKE_FRAMEWORK_DIR}/project/project.cmake)

# --------------------- Source -----------------------

set (SOURCEFILES
  src/main.cpp
)

set (HEADERFILES
)

# --------------------- Dependencies -----------------------

add_external(glm 
	GIT_REPOSITORY https://github.com/g-truc/glm.git
)
add_definitions(-DGLM_FORCE_INLINE)

set(MinVRArgs -DCMAKE_DEBUG_POSTFIX=d -DWITH_PLUGIN_GLFW=ON -DWITH_PLUGIN_OPENGL=ON -DWITH_PLUGIN_THREADING=ON -DWITH_PLUGIN_OPENVR=ON -DWITH_ITEST_OPENGL_SHADERPIPELINE=ON)
add_external(minvr_beta 
	GIT_REPOSITORY https://github.com/MinVR/MinVR.git
	GIT_TAG beta
	CMAKE_ARGS MinVRArgs
	LIB_NAME MinVR
)

include_directories(
	${external_dir}/minvr_beta/build/install/include/MinVR-2.0
)

link_directories(
	${external_dir}/minvr_beta/build/install/lib/MinVR-2.0
)

include(${external_dir}/OpenGL/OpenGL.cmake)
useOpenGL()
useGLEW()

# --------------------- Executable -----------------------

addStandardExecutable()
