add_executable(constBin_test constBin_test.cpp)
target_link_libraries(constBin_test Catch BinaryLib constBinLib)

target_include_directories(constBin_test PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
    )

add_test(NAME DataTest COMMAND constBin_test)

add_definitions( -DTESTFILE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
