message(STATUS "looked for castem library.")

include(cmake/modules/castem-unix-type.cmake)

if(CASTEM_INSTALL_PATH)
  set(CASTEMHOME "${CASTEM_INSTALL_PATH}")
else(CASTEM_INSTALL_PATH)
  set(CASTEMHOME $ENV{CASTEMHOME})
endif(CASTEM_INSTALL_PATH)

if(CASTEMHOME)
    find_path(CASTEM_INCLUDE_DIR castem.h
              HINTS ${CASTEMHOME}/include)
    if(CASTEM_INCLUDE_DIR STREQUAL "CASTEM_INCLUDE_DIR-NOTFOUND")
    	message(FATAL_ERROR "castem.h not found")
    endif(CASTEM_INCLUDE_DIR STREQUAL "CASTEM_INCLUDE_DIR-NOTFOUND")
    message(STATUS "Include file detected: [${CASTEM_INCLUDE_DIR}].")
    set(CASTEM_ROOT "${CASTEMHOME}")
    add_definitions("-DCASTEM_ROOT=\\\"\"${CASTEMHOME}\"\\\"")
    add_definitions("-DHAVE_CASTEM=1")
    set(HAVE_CASTEM ON)
else(CASTEMHOME)
  message(WARNING "no CASTEMHOME defined")
endif(CASTEMHOME)
