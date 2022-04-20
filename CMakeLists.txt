cmake_minimum_required(VERSION 3.18)

project(SCHED
  LANGUAGES CXX
  VERSION 0.1.0
)

# global settings

include(GNUInstallDirs)
include(GenerateExportHeader)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(UNIX)
  # assume it's a GNU-like compiler (GCC, clang)
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
endif()


# options

option(BUILD_SHARED_LIBS "Build the library as a shared library" ON)

if(UNIX)
  option(SCHED_DEV_ENABLE_ASAN "Enable Address Sanitizer" OFF)
endif()


# dependencies

find_package(Threads REQUIRED)
find_package(fmt REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(lqp REQUIRED)


# library

add_library(sched0
  # common
  library/common/GenericSchedule.cc
  library/common/Graph.cc
  library/common/Ids.cc
  library/common/IdsFmt.cc
  library/common/Instance.cc
  library/common/MakespanCriterion.cc
  library/common/Random.cc
  library/common/Range.cc
  library/common/RawId.cc
  library/common/Time.cc

  # para
  library/para/CombineAlgorithm.cc
  library/para/FirstFitDecreasing.cc
  library/para/GenericParallelGenerator.cc
  library/para/LdmAlgorithm.cc
  library/para/ListfitAlgorithm.cc
  library/para/LptAlgorithm.cc
  library/para/MultifitAlgorithm.cc
  library/para/NonUniformParallelGenerator.cc
  library/para/ParallelInstance.cc
  library/para/ParallelJob.cc
  library/para/ParallelOptimalAlgorithm.cc
  library/para/ParallelSchedule.cc
  library/para/SlackAlgorithm.cc

  # shop
  library/shop/AutocorrelationAnalysis.cc
  library/shop/BasicInstance.cc
  library/shop/BasicNeighborhood.cc
  library/shop/Enumerator.cc
  library/shop/Export.cc
  library/shop/FlexibleJobShopInstance.cc
  library/shop/GeneralFlexibleJobShopInstance.cc
  library/shop/GeneralJobListEngine.cc
  library/shop/GeneralJobShopSchedule.cc
  library/shop/Import.cc
  library/shop/InputConversion.cc
  library/shop/InputFmt.cc
  library/shop/InsertNeighborhood.cc
  library/shop/JobListEngine.cc
  library/shop/JobListInput.cc
  library/shop/JobShopCriticalPath.cc
  library/shop/JobShopSchedule.cc
  library/shop/JobShopInstance.cc
  library/shop/MachineListEngine.cc
  library/shop/MachineListInput.cc
  library/shop/NeutralityRateAnalysis.cc
  library/shop/OperationListEngine.cc
  library/shop/OperationListImprovedNeighborhood.cc
  library/shop/OperationListInput.cc
  library/shop/RandomSample.cc
  library/shop/RandomNeighborSample.cc
  library/shop/RandomWalk.cc
  library/shop/ReverseNeighborhood.cc
  library/shop/RowNeighborhood.cc
  library/shop/SimpleDescent.cc
  library/shop/SwapNeighborhoord.cc
  library/shop/TabuSearchAlgorithm.cc
  library/shop/TimeListEngine.cc
  library/shop/TimeListInput.cc
  library/shop/TimeListNeighborhood.cc

  # tools
  library/tools/ConcurrentQueue.cc
  library/tools/Image.cc
  library/tools/KahanSum.cc
  library/tools/LinearRegression.cc
  library/tools/Log.cc
  library/tools/Matrix.cc
  library/tools/Statistics.cc
)

generate_export_header(sched0
  BASE_NAME SCHED
  EXPORT_MACRO_NAME SCHED_API
  EXPORT_FILE_NAME "sched/common/Api.h"
  STATIC_DEFINE STATIC
)

target_compile_features(sched0
  PUBLIC
    cxx_std_17
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(sched0
    PUBLIC
      SCHED_STATIC
  )
endif()

set_target_properties(sched0
  PROPERTIES
    EXPORT_NAME "sched"
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_include_directories(sched0
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/sched/common>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(sched0
  PRIVATE
    nlohmann_json::nlohmann_json
  PUBLIC
    Threads::Threads
    fmt::fmt-header-only
    lqp::lqp
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1")
    message(STATUS "Using explicit C++ std::filesystem library (GNU)")
    target_link_libraries(sched0
      PUBLIC
        stdc++fs
    )
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
    message(STATUS "Using explicit C++ std::filesystem library (clang)")
    target_link_libraries(sched0
      PUBLIC
        stdc++fs
    )
  endif()
endif()

if(NOT BUILD_SHARED_LIBS)
  set_property(TARGET sched0 PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()

# install

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/sched"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  TARGETS sched0
  EXPORT schedTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/sched/common/Api.h"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/sched/common"
)

install(
  EXPORT schedTargets
  NAMESPACE sched::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/sched"
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/schedConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/schedConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/schedConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/sched"
)
