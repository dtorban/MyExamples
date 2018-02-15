project (EigenExample)

include(${CMAKE_FRAMEWORK_DIR}/project/project.cmake)

# --------------------- Source -----------------------

set (SOURCEFILES
  src/main.cpp
)

set (HEADERFILES
)

# --------------------- Executable -----------------------

add_external(Eigen3
	GIT_REPOSITORY https://github.com/eigenteam/eigen-git-mirror.git
)

include_directories(
	${external_dir}/Eigen3/build/install/include/eigen3
)

addStandardExecutable()

