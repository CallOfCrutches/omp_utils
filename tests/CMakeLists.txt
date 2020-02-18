
set(tests_sources
    omp_range_tests.cpp
    omp_reversed_tests.cpp
)

add_executable(omp_utils-unit-tests ${tests_sources})

source_group(Sources\\tests FILES ${tests_sources})

target_link_libraries(omp_utils-unit-tests 
    PRIVATE 
        OmpUtils::omp_utils
        gtest
        gtest_main
)
