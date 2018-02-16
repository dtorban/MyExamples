project (MinVRExample)

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

set(MinVRArgs -DCMAKE_DEBUG_POSTFIX=d -DMINVR_THREADING_PLUGIN=ON)
add_external(minvr 
	GIT_REPOSITORY https://github.com/MinVR/MinVR.git
	GIT_TAG v0.3
	CMAKE_ARGS MinVRArgs
	LIB_NAME MinVR
)

include(${external_dir}/OpenGL/OpenGL.cmake)
useOpenGL()
useGLEW()

# --------------------- Executable -----------------------

addStandardExecutable()
