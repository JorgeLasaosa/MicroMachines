cmake_minimum_required(VERSION 2.8)
project( pengo_exe )


# Usar C++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-O3 -std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-O3 -std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -Iinclude -Ilib/include -Ilib/include/freetype2")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -O2 -Iinclude -Ilib/include -Ilib/include/freetype2")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# Nuestro codigo
file(GLOB SOURCES
    src/*.cpp
)
file(GLOB LIBS
	lib/*.a
	lib/libIrrKlang.so
	lib/libassimp.so
)

# include_directories(include)
include_directories(pengo include lib/include)

add_executable( pengo_exe ${SOURCES})

target_link_libraries(pengo_exe ${LIBS}  )


target_link_libraries(pengo_exe -lGL -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor)
