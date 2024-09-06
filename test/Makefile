# JustCoderdev Makefile for C projects v5

PROGRAM_NAME = tester

SRC_FILES  = test.c
OBJ_FILES  = $(patsubst %.c,%.o,${SRC_FILES})
SHS_FILES  = ../shs/shs.c
CORE_FILES = ../core/core_bit.c \
             ../core/core_buff.c \
             ../core/core_logger.c \
             ../core/core_memdeb.c \
             ../core/core_net.c \
             ../core/core_str.c \
             ../core/core_stuff.c

CC = gcc
CCFLAGS = -xc -std=c89 -ansi -pedantic-errors -pedantic \
		 -Wall -Wextra -Werror -Wshadow -Wpointer-arith \
		 -Wcast-qual -Wcast-align -Wstrict-prototypes -Wmissing-prototypes \
		 -Wconversion -g

IFLAGS = -I./ -I./lib/include -I../include
LDFLAGS = -L./
#RAYFLAGS = -lGL -lm -lpthread -ldl -lX11 \
#		-Wl,-rpath=./lib/raylib/ -L./lib/raylib -lraylib
##-lXrandr -lXinerama -lXi -lXcursor

#-DDEBUG_MEMDEB_ENABLE=1
#-DDEBUG_STRING_ENABLE=1
#-DDEBUG_ENABLE=1
DFLAGS = -DSGR_ENABLE=1
FLAGS = $(CCFLAGS) $(IFLAGS) $(LDFLAGS) $(RAYFLAGS) $(DFLAGS)

# local: build run
#build_so:
#	@echo "Compiling... (auto)"
#	@mkdir -p bin
#	$(CC) $(CCFLAGS) $(IFLAGS) $(RAYFLAGS) $(DFLAGS) \
#		-fPIC -shared hotfile.c $(CORE_FILES) -o bin/libhotfile.so

# build:
# 	@echo "Compiling... "
# 	@mkdir -p bin
# 	$(CC) $(FLAGS) $(SRC_FILES) $(CORE_FILES) $(SHS_FILES) -o bin/${PROGRAM_NAME}

# run:
# 	@echo -e "Running...\n"
# 	@chmod +x bin/$(PROGRAM_NAME)
# 	@cd bin && ./$(PROGRAM_NAME)

local_test: build_test run_test
build_test:
	@echo "Compiling... (test)"
	@mkdir -p bin
	@cp -r shs_test_files bin/shs_test_files
	$(CC) $(FLAGS) test.c $(SHS_FILES) $(CORE_FILES) -o bin/${PROGRAM_NAME}_test

run_test:
	@echo -e "Running... (test)\n"
	@chmod +x bin/$(PROGRAM_NAME)_test
	@cd bin && ./$(PROGRAM_NAME)_test


.PHONY: clean
clean:
	@echo "Cleaning..."
	@rm -rf ./bin


# raycompile: rayclean raybuild
# rayclean:
# 	cd lib/sources/raylib/src && make clean

# raybuild:
# 	cd lib/sources/raylib/src && make PLATFORM=PLATFORM_DESKTOP GLFW_LINUX_ENABLE_WAYLAND=OFF

# rayshared:
# 	cd lib/sources/raylib/src && make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED GLFW_LINUX_ENABLE_WAYLAND=OFF
