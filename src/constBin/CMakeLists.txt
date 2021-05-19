add_library(constBinLib STATIC
    absConstBinary.cpp
    absConstBinary.h
    MacroMage.h
    )

target_link_libraries(constBinLib BinaryLib includes_lib)

set(INCLUDE_LIST
    "${CMAKE_SOURCE_DIR}/include/map.h"
    )

set_target_properties(constBinLib PROPERTIES PREFIX "")

add_custom_target(includes SOURCES ${INCLUDE_LIST})

install(FILES
    absConstBinary.h
    MacroMage.h
    ${INCLUDE_LIST}
    DESTINATION include/constBin)
