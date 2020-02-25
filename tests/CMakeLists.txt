
set(tests_sources
    make_loaded_list_tests.cpp
    omp_enumerate_tests.cpp
    omp_in_tests.cpp
    omp_range_tests.cpp
    omp_reversed_tests.cpp
)

add_executable(omp_utils-unit-tests ${tests_sources})

source_group(Sources\\tests FILES ${tests_sources})

target_link_libraries(omp_utils-unit-tests 
    PRIVATE 
        OmpUtils::omp_utils

        gmock
        gmock_main
        gtest
        gtest_main
)
