cmake_minimum_required(VERSION 3.16)

project(OmpUtils VERSION 0.0.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(OMP_UTILS_BUILD_TESTS "Build tests" ON)
option(OMP_UTILS_BUILD_EXAMPLES "Build examples" ON)
option(OMP_UTILS_SHOW_IDE_SUPPORT "Set ide support for header files" ON)

###################################################################################################

add_library(omp-utils INTERFACE)
target_include_directories(omp-utils INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set(omp_utils_headers
    include/omp/utils/in.h
    include/omp/utils/range.h
    include/omp/utils/reversed.h
    include/omp/utils/enumerate.h
    include/omp/utils/make_loaded_list.h
)

add_library(OmpUtils::omp-utils ALIAS omp-utils)

if(OMP_UTILS_SHOW_IDE_SUPPORT)
    add_custom_target(omp-utils-ide SOURCES ${omp_utils_headers})
endif()

###################################################################################################

install(DIRECTORY include/omp/utils DESTINATION include)

install(TARGETS omp-utils EXPORT OmpUtilsConfig)
install(EXPORT OmpUtilsConfig NAMESPACE OmpUtils:: DESTINATION share/OmpUtils/cmake)

include(CMakePackageConfigHelpers)

write_basic_package_version_file("${PROJECT_BINARY_DIR}/OmpUtilsConfigVersion.cmake"
    VERSION ${PROJECT_VERSION} 
    COMPATIBILITY AnyNewerVersion
)

install(FILES "${PROJECT_BINARY_DIR}/OmpUtilsConfigVersion.cmake" DESTINATION share/OmpUtils/cmake)

###################################################################################################

if(OMP_UTILS_BUILD_TESTS)
    add_subdirectory(third-party/gtest)
    add_subdirectory(tests)
endif()

###################################################################################################

if(OMP_UTILS_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()