add_library(dynBinLib STATIC
    absDynamicBinary.cpp
    absDynamicBinary.h
    )

target_link_libraries(dynBinLib BinaryLib)


set_target_properties(dynBinLib PROPERTIES PREFIX "")

install(FILES
    absDynamicBinary.h
    DESTINATION include/dynBin)
