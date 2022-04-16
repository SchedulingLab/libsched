include (CMakeFindDependencyMacro)

get_filename_component (SCHED_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)
include("${SCHED_CMAKE_DIR}/schedTargets.cmake")

find_dependency(Threads)
find_dependency(fmt)

if(NOT BUILD_SHARED_LIBS)
  find_dependency(nlohmann_json)
endif ()

set(SCHED_FOUND TRUE)

include("${SCHED_CMAKE_DIR}/schedConfigVersion.cmake")

message(STATUS "Found sched (version \"${PACKAGE_VERSION}\") in ${CMAKE_CURRENT_LIST_DIR}")
