#
# **************************************************************
# *                Simple C++ Makefile Template                *
# *                                                            *
# * Author: Arash Partow (2003)                                *
# * URL: http://www.partow.net/programming/makefile/index.html *
# *                                                            *
# * Copyright notice:                                          *
# * Free use of this C++ Makefile template is permitted under  *
# * the guidelines and in accordance with the the MIT License  *
# * http://www.opensource.org/licenses/MIT                     *
# *                                                            *
# **************************************************************
#
UNAME_R := $(uname -r | grep microsoft)
CXX      := g++
WARNINGS := -Wall -Wextra -Wswitch-enum -Wstack-protector -Wdate-time -Walloc-zero -Walloca
WARNINGS_NO := -Wno-shadow -Wno-unused-parameter
IF_WINDOWS := -fno-stack-protector
CXXFLAGS := -std=c++17 -pedantic-errors $(WARNINGS) $(WARNINGS_NO)
LDFLAGS  := -lstdc++ -lm
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TARGET   := main
INCLUDE  := -Iinclude/ -I/usr/local/include
SRC      :=                      \
	 $(wildcard src/*.cpp)         \


OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES \
         := $(OBJECTS:.o=.d)

#for some reason wsl doesn't work without stacksmashing removed?
#ifeq ($(UNAME_R),)
#	CXXFLAGS := $(CXXFLAGS) $(IF_WINDOWS)
#endif

all: build $(APP_DIR)/$(TARGET)
	@echo all

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

-include $(DEPENDENCIES)

.PHONY: all build clean debug release info

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*

info:
	@echo "[*] Application dir: ${APP_DIR}     "
	@echo "[*] Object dir:      ${OBJ_DIR}     "
	@echo "[*] Sources:         ${SRC}         "
	@echo "[*] Objects:         ${OBJECTS}     "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"
run:
	@echo "running program"
	@exec build/apps/${TARGET}
gdb:
	gdb build/apps/${TARGET}
