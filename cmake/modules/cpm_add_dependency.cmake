include_guard(GLOBAL)

include(FetchContent)

function(_cpm_add_dependency dependency_name dependency_target git_repository git_tag)
	if(TARGET ${dependency_target})
		message(WARNING "target '${dependency_target}' already exists; skipping FetchContent setup")
		return()
	endif()

	FetchContent_Declare(
		${dependency_name}
		GIT_REPOSITORY ${git_repository}
		GIT_TAG        ${git_tag}
		GIT_SHALLOW    TRUE
		EXCLUDE_FROM_ALL
	)

	FetchContent_MakeAvailable(${dependency_name})
endfunction()

function(cpm_add_cli11)
    _cpm_add_dependency(
        CLI11
        CLI11::CLI11
        https://github.com/CLIUtils/CLI11.git
        v2.6.2
    )
endfunction()

function(cpm_add_googletest)
	_cpm_add_dependency(
		googletest
		GTest::gtest
		https://github.com/google/googletest.git
		v1.17.0
	)
endfunction()

function(cpm_add_raylib)
	_cpm_add_dependency(
		raylib
		raylib
		https://github.com/raysan5/raylib.git
		6.0
	)
endfunction()

function(cpm_add_spdlog)
	_cpm_add_dependency(
		spdlog
		spdlog::spdlog
		https://github.com/gabime/spdlog.git
		v1.17.0
	)
endfunction()
