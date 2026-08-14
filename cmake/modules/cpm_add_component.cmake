include_guard(GLOBAL)

function(cpm_add_component component_name)
    cmake_parse_arguments(ARG "" "" "SOURCES;TEST_SOURCES;DEPENDENCIES" ${ARGN})

    set(lib_target ${component_name})
    set(test_target ${component_name}_tests)

    if(NOT ARG_SOURCES)
        message(WARNING "component '${component_name}' was given no sources, skipping component setup")
        return()
    endif()

    add_library(${lib_target} ${ARG_SOURCES})
    target_include_directories(${lib_target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

    if(ARG_DEPENDENCIES)
        target_link_libraries(${lib_target} PUBLIC ${ARG_DEPENDENCIES})
    endif()

    if (CPM_BUILD_TESTS AND ARG_TEST_SOURCES)
        add_executable(${test_target} ${ARG_TEST_SOURCES})
        target_link_libraries(${test_target} ${lib_target} GTest::gtest_main)
        gtest_discover_tests(${test_target})
    endif()
endfunction()
