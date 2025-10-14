cmake_minimum_required(VERSION 3.10)
project(4basic)
add_executable(4basic
	"src/main.c"
	"src/interface.h" "src/interface.c"
	"src/input.h" "src/input.c"
	"src/linedisplay.h" "src/linedisplay.c"
)
