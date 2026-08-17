include_guard(GLOBAL)

function(cpm_add_component component_name)
    cmake_parse_arguments(ARG "" "" "SOURCES;TEST_SOURCES;DEPENDENCIES" ${ARGN})

    set(lib_target ${component_name})

    if(NOT ARG_SOURCES)
        message(WARNING "component '${component_name}' was given no sources, skipping component setup")
        return()
    endif()

    add_library(${lib_target} ${ARG_SOURCES})
    target_include_directories(${lib_target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_include_directories(${lib_target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

    if(ARG_DEPENDENCIES)
        target_link_libraries(${lib_target} PUBLIC ${ARG_DEPENDENCIES})
    endif()

    if (SOLVER_BUILD_TESTS AND ARG_TEST_SOURCES)
        target_sources(unit_tests PRIVATE ${ARG_TEST_SOURCES})
        target_link_libraries(unit_tests ${lib_target})
    endif()
endfunction()
