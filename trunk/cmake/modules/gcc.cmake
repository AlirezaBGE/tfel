include(cmake/modules/common-compiler-flags.cmake)

tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wdisabled-optimization")
if(NOT i586-mingw32msvc_COMPILER)
  tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "fvisibility=hidden")
  tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "fvisibility-inlines-hidden")
endif(NOT i586-mingw32msvc_COMPILER)
tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "march=native")
tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "ftree-vectorize")
tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "ffast-math")

set(OPTIMISATION_FLAGS "-DNO_RUNTIME_CHECK_BOUNDS ${OPTIMISATION_FLAGS}")

if((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))
  set(OPTIMISATION_FLAGS "-O2 -DNDEBUG ${OPTIMISATION_FLAGS}")
endif((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions("-g")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

if(HAVE_FORTRAN)
  include(cmake/modules/gnu-fortran-compiler.cmake)
endif(HAVE_FORTRAN)
