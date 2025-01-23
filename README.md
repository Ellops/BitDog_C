# BitDog_C
Um repositório simples com varias bibliotecas de software para o hardware bitdoglab

Basta adicionar ao CMakeLists.txt principal:

add_subdirectory(libs/pico-matrix)

target_link_libraries(nome-do-projeto
    pico_matrix
)
