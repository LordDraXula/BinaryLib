#add_subdirectory(constBin)
#add_subdirectory(dynBin)
add_subdirectory(examples)

set(BINLIB_HEADERS Endianconversion.h
    Definitions.h
    IBinary.h
    CRCcalc.h
    BinaryFileHandler.h
    absBinary.h
    IDefinitions.h
    EnumDefinitions.h
    DTSM.h
    Reflection.h
    BinaryMember.h
    )

set(CONSTBIN_HEADERS constBin/absConstBinary.h
    constBin/MacroMage.h)

set(DYNBIN_HEADERS dynBin/absDynamicBinary.h)

set(BINLIB_CPP EndianConversion.cpp
    IBinary.cpp
    CRCcalc.cpp
    BinaryFileHandler.cpp
    absBinary.cpp
    IDefinitions.cpp
    DTSM.cpp
    Reflection.cpp
    constBin/absConstBinary.cpp
    )

set(CONSTBIN_CPP constBin/absConstBinary.cpp)

set(DYNBIN_CPP dynBin/absDynamicBinary.cpp)

add_library(BinaryLib SHARED
    ${BINLIB_HEADERS}
    ${BINLIB_CPP}
    ${CONSTBIN_HEADERS}
    ${CONSTBIN_CPP}
    ${DYNBIN_HEADERS}
    ${DYNBIN_CPP}
    )

set_target_properties(BinaryLib PROPERTIES PREFIX "")

set(INCLUDE_LIST
    "${CMAKE_SOURCE_DIR}/include/map.h"
    )
add_custom_target(includes SOURCES ${INCLUDE_LIST})

install(FILES
    ${BINLIB_HEADERS}
    DESTINATION include)
install(FILES
    ${CONSTBIN_HEADERS}
    ${INCLUDE_LIST}
    DESTINATION include/constBin)
install(FILES
    ${DYNBIN_HEADERS}
    DESTINATION include/dynBin)

target_include_directories(BinaryLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
