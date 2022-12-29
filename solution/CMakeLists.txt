file(GLOB_RECURSE sources CONFIGURE_DEPENDS
	src/*.c
	include/*.h
)

add_executable(lab9 ${sources})
target_include_directories(lab9 PRIVATE include)
