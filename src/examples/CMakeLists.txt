set(EXAMPLE_LIST
    exampleClass.cpp
    exampleClass.h
    )

add_custom_target(example SOURCES
    ${EXAMPLE_LIST})

install(FILES
    ${EXAMPLE_LIST}
    DESTINATION example
    )
