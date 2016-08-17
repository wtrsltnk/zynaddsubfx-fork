
add_definitions(-DSOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

set(test_lib
    zynaddsubfx_core
    zynaddsubfx_nio
    ${ZLIB_LIBRARY}
    ${FFTW_LIBRARIES}
    ${MXML_LIBRARIES}
    ${NIO_LIBRARIES}
    ${AUDIO_LIBRARIES}
    pthread
    )

file(GLOB TESTFILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(file ${TESTFILES})
    get_filename_component(filename ${file} NAME FALSE)
    string(REGEX REPLACE ".cpp\$" "" test "${filename}")
    add_executable(${test} ${file})
    target_link_libraries(${test} ${test_lib})
    if (CompileTests)
        add_test(
            NAME
                ${test}
            WORKING_DIRECTORY
                "${CMAKE_CURRENT_BINARY_DIR}/.."
            COMMAND
                ${test}
            )
    endif()
endforeach()
