
add_executable(omp_utils-unit-tests test.cpp)


target_link_libraries(omp_utils-unit-tests 
    PRIVATE 
        OmpUtils::omp_utils
        gtest
        gtest_main
)