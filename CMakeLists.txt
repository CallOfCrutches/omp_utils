cmake_minimum_required(VERSION 3.16)

project(OmpUtils VERSION 0.0.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

###################################################################################################

add_library(omp_utils INTERFACE)
target_include_directories(omp_utils INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_library(OmpUtils::omp_utils ALIAS omp_utils)

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

add_subdirectory(third-party/gtest)
add_subdirectory(tests)