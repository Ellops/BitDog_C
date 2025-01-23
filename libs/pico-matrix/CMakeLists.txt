add_library(pico_matrix STATIC
    matrix.c
    matrix.h
)

target_include_directories(pico_matrix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(pico_matrix
    pico_stdlib
    hardware_pio
)