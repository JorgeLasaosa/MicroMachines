# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.0

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:
.PHONY : .NOTPARALLEL

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/roberto/Roberto_Universidad/Videojuegos/git/Pengo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/roberto/Roberto_Universidad/Videojuegos/git/Pengo

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/cmake-gui -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/roberto/Roberto_Universidad/Videojuegos/git/Pengo/CMakeFiles /home/roberto/Roberto_Universidad/Videojuegos/git/Pengo/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/roberto/Roberto_Universidad/Videojuegos/git/Pengo/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named pengo_exe

# Build rule for target.
pengo_exe: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 pengo_exe
.PHONY : pengo_exe

# fast build rule for target.
pengo_exe/fast:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/build
.PHONY : pengo_exe/fast

src/Diamondblock.o: src/Diamondblock.cpp.o
.PHONY : src/Diamondblock.o

# target to build an object file
src/Diamondblock.cpp.o:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Diamondblock.cpp.o
.PHONY : src/Diamondblock.cpp.o

src/Diamondblock.i: src/Diamondblock.cpp.i
.PHONY : src/Diamondblock.i

# target to preprocess a source file
src/Diamondblock.cpp.i:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Diamondblock.cpp.i
.PHONY : src/Diamondblock.cpp.i

src/Diamondblock.s: src/Diamondblock.cpp.s
.PHONY : src/Diamondblock.s

# target to generate assembly for a file
src/Diamondblock.cpp.s:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Diamondblock.cpp.s
.PHONY : src/Diamondblock.cpp.s

src/FloatingText.o: src/FloatingText.cpp.o
.PHONY : src/FloatingText.o

# target to build an object file
src/FloatingText.cpp.o:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/FloatingText.cpp.o
.PHONY : src/FloatingText.cpp.o

src/FloatingText.i: src/FloatingText.cpp.i
.PHONY : src/FloatingText.i

# target to preprocess a source file
src/FloatingText.cpp.i:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/FloatingText.cpp.i
.PHONY : src/FloatingText.cpp.i

src/FloatingText.s: src/FloatingText.cpp.s
.PHONY : src/FloatingText.s

# target to generate assembly for a file
src/FloatingText.cpp.s:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/FloatingText.cpp.s
.PHONY : src/FloatingText.cpp.s

src/Game.o: src/Game.cpp.o
.PHONY : src/Game.o

# target to build an object file
src/Game.cpp.o:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Game.cpp.o
.PHONY : src/Game.cpp.o

src/Game.i: src/Game.cpp.i
.PHONY : src/Game.i

# target to preprocess a source file
src/Game.cpp.i:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Game.cpp.i
.PHONY : src/Game.cpp.i

src/Game.s: src/Game.cpp.s
.PHONY : src/Game.s

# target to generate assembly for a file
src/Game.cpp.s:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Game.cpp.s
.PHONY : src/Game.cpp.s

src/GameLevel.o: src/GameLevel.cpp.o
.PHONY : src/GameLevel.o

# target to build an object file
src/GameLevel.cpp.o:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/GameLevel.cpp.o
.PHONY : src/GameLevel.cpp.o

src/GameLevel.i: src/GameLevel.cpp.i
.PHONY : src/GameLevel.i

# target to preprocess a source file
src/GameLevel.cpp.i:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/GameLevel.cpp.i
.PHONY : src/GameLevel.cpp.i

src/GameLevel.s: src/GameLevel.cpp.s
.PHONY : src/GameLevel.s

# target to generate assembly for a file
src/GameLevel.cpp.s:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/GameLevel.cpp.s
.PHONY : src/GameLevel.cpp.s

src/GameObject.o: src/GameObject.cpp.o
.PHONY : src/GameObject.o

# target to build an object file
src/GameObject.cpp.o:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/GameObject.cpp.o
.PHONY : src/GameObject.cpp.o

src/GameObject.i: src/GameObject.cpp.i
.PHONY : src/GameObject.i

# target to preprocess a source file
src/GameObject.cpp.i:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/GameObject.cpp.i
.PHONY : src/GameObject.cpp.i

src/GameObject.s: src/GameObject.cpp.s
.PHONY : src/GameObject.s

# target to generate assembly for a file
src/GameObject.cpp.s:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/GameObject.cpp.s
.PHONY : src/GameObject.cpp.s

src/Iceblock.o: src/Iceblock.cpp.o
.PHONY : src/Iceblock.o

# target to build an object file
src/Iceblock.cpp.o:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Iceblock.cpp.o
.PHONY : src/Iceblock.cpp.o

src/Iceblock.i: src/Iceblock.cpp.i
.PHONY : src/Iceblock.i

# target to preprocess a source file
src/Iceblock.cpp.i:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Iceblock.cpp.i
.PHONY : src/Iceblock.cpp.i

src/Iceblock.s: src/Iceblock.cpp.s
.PHONY : src/Iceblock.s

# target to generate assembly for a file
src/Iceblock.cpp.s:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Iceblock.cpp.s
.PHONY : src/Iceblock.cpp.s

src/Menu.o: src/Menu.cpp.o
.PHONY : src/Menu.o

# target to build an object file
src/Menu.cpp.o:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Menu.cpp.o
.PHONY : src/Menu.cpp.o

src/Menu.i: src/Menu.cpp.i
.PHONY : src/Menu.i

# target to preprocess a source file
src/Menu.cpp.i:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Menu.cpp.i
.PHONY : src/Menu.cpp.i

src/Menu.s: src/Menu.cpp.s
.PHONY : src/Menu.s

# target to generate assembly for a file
src/Menu.cpp.s:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Menu.cpp.s
.PHONY : src/Menu.cpp.s

src/Player.o: src/Player.cpp.o
.PHONY : src/Player.o

# target to build an object file
src/Player.cpp.o:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Player.cpp.o
.PHONY : src/Player.cpp.o

src/Player.i: src/Player.cpp.i
.PHONY : src/Player.i

# target to preprocess a source file
src/Player.cpp.i:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Player.cpp.i
.PHONY : src/Player.cpp.i

src/Player.s: src/Player.cpp.s
.PHONY : src/Player.s

# target to generate assembly for a file
src/Player.cpp.s:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Player.cpp.s
.PHONY : src/Player.cpp.s

src/ResourceManager.o: src/ResourceManager.cpp.o
.PHONY : src/ResourceManager.o

# target to build an object file
src/ResourceManager.cpp.o:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/ResourceManager.cpp.o
.PHONY : src/ResourceManager.cpp.o

src/ResourceManager.i: src/ResourceManager.cpp.i
.PHONY : src/ResourceManager.i

# target to preprocess a source file
src/ResourceManager.cpp.i:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/ResourceManager.cpp.i
.PHONY : src/ResourceManager.cpp.i

src/ResourceManager.s: src/ResourceManager.cpp.s
.PHONY : src/ResourceManager.s

# target to generate assembly for a file
src/ResourceManager.cpp.s:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/ResourceManager.cpp.s
.PHONY : src/ResourceManager.cpp.s

src/Shader.o: src/Shader.cpp.o
.PHONY : src/Shader.o

# target to build an object file
src/Shader.cpp.o:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Shader.cpp.o
.PHONY : src/Shader.cpp.o

src/Shader.i: src/Shader.cpp.i
.PHONY : src/Shader.i

# target to preprocess a source file
src/Shader.cpp.i:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Shader.cpp.i
.PHONY : src/Shader.cpp.i

src/Shader.s: src/Shader.cpp.s
.PHONY : src/Shader.s

# target to generate assembly for a file
src/Shader.cpp.s:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Shader.cpp.s
.PHONY : src/Shader.cpp.s

src/Snobee.o: src/Snobee.cpp.o
.PHONY : src/Snobee.o

# target to build an object file
src/Snobee.cpp.o:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Snobee.cpp.o
.PHONY : src/Snobee.cpp.o

src/Snobee.i: src/Snobee.cpp.i
.PHONY : src/Snobee.i

# target to preprocess a source file
src/Snobee.cpp.i:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Snobee.cpp.i
.PHONY : src/Snobee.cpp.i

src/Snobee.s: src/Snobee.cpp.s
.PHONY : src/Snobee.s

# target to generate assembly for a file
src/Snobee.cpp.s:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Snobee.cpp.s
.PHONY : src/Snobee.cpp.s

src/SnobeeEgg.o: src/SnobeeEgg.cpp.o
.PHONY : src/SnobeeEgg.o

# target to build an object file
src/SnobeeEgg.cpp.o:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/SnobeeEgg.cpp.o
.PHONY : src/SnobeeEgg.cpp.o

src/SnobeeEgg.i: src/SnobeeEgg.cpp.i
.PHONY : src/SnobeeEgg.i

# target to preprocess a source file
src/SnobeeEgg.cpp.i:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/SnobeeEgg.cpp.i
.PHONY : src/SnobeeEgg.cpp.i

src/SnobeeEgg.s: src/SnobeeEgg.cpp.s
.PHONY : src/SnobeeEgg.s

# target to generate assembly for a file
src/SnobeeEgg.cpp.s:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/SnobeeEgg.cpp.s
.PHONY : src/SnobeeEgg.cpp.s

src/SpriteFrame.o: src/SpriteFrame.cpp.o
.PHONY : src/SpriteFrame.o

# target to build an object file
src/SpriteFrame.cpp.o:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/SpriteFrame.cpp.o
.PHONY : src/SpriteFrame.cpp.o

src/SpriteFrame.i: src/SpriteFrame.cpp.i
.PHONY : src/SpriteFrame.i

# target to preprocess a source file
src/SpriteFrame.cpp.i:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/SpriteFrame.cpp.i
.PHONY : src/SpriteFrame.cpp.i

src/SpriteFrame.s: src/SpriteFrame.cpp.s
.PHONY : src/SpriteFrame.s

# target to generate assembly for a file
src/SpriteFrame.cpp.s:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/SpriteFrame.cpp.s
.PHONY : src/SpriteFrame.cpp.s

src/SpriteRenderer.o: src/SpriteRenderer.cpp.o
.PHONY : src/SpriteRenderer.o

# target to build an object file
src/SpriteRenderer.cpp.o:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/SpriteRenderer.cpp.o
.PHONY : src/SpriteRenderer.cpp.o

src/SpriteRenderer.i: src/SpriteRenderer.cpp.i
.PHONY : src/SpriteRenderer.i

# target to preprocess a source file
src/SpriteRenderer.cpp.i:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/SpriteRenderer.cpp.i
.PHONY : src/SpriteRenderer.cpp.i

src/SpriteRenderer.s: src/SpriteRenderer.cpp.s
.PHONY : src/SpriteRenderer.s

# target to generate assembly for a file
src/SpriteRenderer.cpp.s:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/SpriteRenderer.cpp.s
.PHONY : src/SpriteRenderer.cpp.s

src/TextRenderer.o: src/TextRenderer.cpp.o
.PHONY : src/TextRenderer.o

# target to build an object file
src/TextRenderer.cpp.o:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/TextRenderer.cpp.o
.PHONY : src/TextRenderer.cpp.o

src/TextRenderer.i: src/TextRenderer.cpp.i
.PHONY : src/TextRenderer.i

# target to preprocess a source file
src/TextRenderer.cpp.i:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/TextRenderer.cpp.i
.PHONY : src/TextRenderer.cpp.i

src/TextRenderer.s: src/TextRenderer.cpp.s
.PHONY : src/TextRenderer.s

# target to generate assembly for a file
src/TextRenderer.cpp.s:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/TextRenderer.cpp.s
.PHONY : src/TextRenderer.cpp.s

src/Texture.o: src/Texture.cpp.o
.PHONY : src/Texture.o

# target to build an object file
src/Texture.cpp.o:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Texture.cpp.o
.PHONY : src/Texture.cpp.o

src/Texture.i: src/Texture.cpp.i
.PHONY : src/Texture.i

# target to preprocess a source file
src/Texture.cpp.i:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Texture.cpp.i
.PHONY : src/Texture.cpp.i

src/Texture.s: src/Texture.cpp.s
.PHONY : src/Texture.s

# target to generate assembly for a file
src/Texture.cpp.s:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Texture.cpp.s
.PHONY : src/Texture.cpp.s

src/Wallblock.o: src/Wallblock.cpp.o
.PHONY : src/Wallblock.o

# target to build an object file
src/Wallblock.cpp.o:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Wallblock.cpp.o
.PHONY : src/Wallblock.cpp.o

src/Wallblock.i: src/Wallblock.cpp.i
.PHONY : src/Wallblock.i

# target to preprocess a source file
src/Wallblock.cpp.i:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Wallblock.cpp.i
.PHONY : src/Wallblock.cpp.i

src/Wallblock.s: src/Wallblock.cpp.s
.PHONY : src/Wallblock.s

# target to generate assembly for a file
src/Wallblock.cpp.s:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/Wallblock.cpp.s
.PHONY : src/Wallblock.cpp.s

src/main.o: src/main.cpp.o
.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) -f CMakeFiles/pengo_exe.dir/build.make CMakeFiles/pengo_exe.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... pengo_exe"
	@echo "... rebuild_cache"
	@echo "... src/Diamondblock.o"
	@echo "... src/Diamondblock.i"
	@echo "... src/Diamondblock.s"
	@echo "... src/FloatingText.o"
	@echo "... src/FloatingText.i"
	@echo "... src/FloatingText.s"
	@echo "... src/Game.o"
	@echo "... src/Game.i"
	@echo "... src/Game.s"
	@echo "... src/GameLevel.o"
	@echo "... src/GameLevel.i"
	@echo "... src/GameLevel.s"
	@echo "... src/GameObject.o"
	@echo "... src/GameObject.i"
	@echo "... src/GameObject.s"
	@echo "... src/Iceblock.o"
	@echo "... src/Iceblock.i"
	@echo "... src/Iceblock.s"
	@echo "... src/Menu.o"
	@echo "... src/Menu.i"
	@echo "... src/Menu.s"
	@echo "... src/Player.o"
	@echo "... src/Player.i"
	@echo "... src/Player.s"
	@echo "... src/ResourceManager.o"
	@echo "... src/ResourceManager.i"
	@echo "... src/ResourceManager.s"
	@echo "... src/Shader.o"
	@echo "... src/Shader.i"
	@echo "... src/Shader.s"
	@echo "... src/Snobee.o"
	@echo "... src/Snobee.i"
	@echo "... src/Snobee.s"
	@echo "... src/SnobeeEgg.o"
	@echo "... src/SnobeeEgg.i"
	@echo "... src/SnobeeEgg.s"
	@echo "... src/SpriteFrame.o"
	@echo "... src/SpriteFrame.i"
	@echo "... src/SpriteFrame.s"
	@echo "... src/SpriteRenderer.o"
	@echo "... src/SpriteRenderer.i"
	@echo "... src/SpriteRenderer.s"
	@echo "... src/TextRenderer.o"
	@echo "... src/TextRenderer.i"
	@echo "... src/TextRenderer.s"
	@echo "... src/Texture.o"
	@echo "... src/Texture.i"
	@echo "... src/Texture.s"
	@echo "... src/Wallblock.o"
	@echo "... src/Wallblock.i"
	@echo "... src/Wallblock.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

