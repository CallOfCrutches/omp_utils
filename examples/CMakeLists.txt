
set(examples_sources
    omp/utils/enumerate_example.cpp
    omp/utils/head_example.cpp
    omp/utils/in_example.cpp
    omp/utils/main.cpp
    omp/utils/make_loaded_list_example.cpp
    omp/utils/range_example.cpp
    omp/utils/reversed_example.cpp
    omp/utils/tuple_map_reduce_example.cpp
)

add_executable(omp-utils-examples ${examples_sources})

source_group(Sources\\examples\\omp\\utils FILES ${examples_sources})

target_link_libraries(omp-utils-examples
    PRIVATE
        omp-utils
)