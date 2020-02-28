cmake_minimum_required(VERSION 3.16)

project(OmpUtils VERSION 0.0.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(OMP_UTILS_SHOW_IDE_SUPPORT "Set ide support for header files" ON)
option(OMP_UTILS_BUILD_TESTS "Build tests" ON)

###################################################################################################

add_library(omp_utils INTERFACE)
target_include_directories(omp_utils INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set(omp_utils_headers
    include/omp/utils/omp_make_loaded_list.h
    include/omp/utils/omp_enumerate.h
    include/omp/utils/omp_in.h
    include/omp/utils/omp_range.h
    include/omp/utils/omp_reversed.h
)

add_library(OmpUtils::omp_utils ALIAS omp_utils)

if(OMP_UTILS_SHOW_IDE_SUPPORT)
    add_custom_target(omp_utils_ide SOURCES ${omp_utils_headers})
endif()

###################################################################################################

install(DIRECTORY include/omp_utils DESTINATION include)

install(TARGETS omp_utils EXPORT OmpUtilsConfig)
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