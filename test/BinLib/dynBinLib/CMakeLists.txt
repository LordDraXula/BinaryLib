add_executable(dynBin_test dynBin_test.cpp)
target_link_libraries(dynBin_test Catch BinaryLib dynBinLib)

target_include_directories(dynBin_test PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
    )

add_test(NAME DataTest COMMAND dynBin_test)
