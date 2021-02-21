
set(tests_sources
    omp/utils/common_tests.cpp
    omp/utils/enumerate_tests.cpp
    omp/utils/head_tests.cpp
    omp/utils/in_tests.cpp
    omp/utils/make_loaded_list_tests.cpp
    omp/utils/range_tests.cpp
    omp/utils/reversed_tests.cpp
    omp/utils/zip_tests.cpp
)

add_executable(omp-utils-unit-tests ${tests_sources})

source_group(Sources\\tests\\omp\\utils FILES ${tests_sources})

target_link_libraries(omp-utils-unit-tests 
    PRIVATE 
        omp-utils

        gmock
        gmock_main
        gtest
        gtest_main
)
