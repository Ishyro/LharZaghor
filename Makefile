# Base directories

BIN_WIN=bin_win
BIN_UNIX=bin
BIN=$(BIN_UNIX)
EXEC=src/clients/graphics/godot_client_spark_holders
TEST=test
SRC=src
INCLUDE=include
SCRIPTS=scripts

# Targets

TARGET_TEST=$(EXEC)/TestLauncher.x86_64
#TARGET_CLIENT_TERMINAL=$(EXEC)/Client_terminal.x86_64

TARGET_SERVER_WIN=$(EXEC)/SparkHolders.Server.exe
TARGET_SERVER_UNIX=$(EXEC)/SparkHolders.Server.x86_64
TARGET_SERVER=$(TARGET_SERVER_UNIX)

# Utilities

MKDIR=mkdir -p
RM=rm -rf

# Compiler and flags

CC=g++
CC_FLAGS=-O3 -pipe -pthread -fpermissive -Wall -Wno-reorder
CC_DEBUG_FLAGS=-O0 -g -ggdb -pipe -pthread -fpermissive -Wall -Wno-reorder
CC_INCLUDES=-I $(INCLUDE)
CC_LIBRARIES=
NCURSES_LIBRARIES=-lncursesw -lformw -lmenuw -lpanelw
TARGET_GODOT=externals/godot/bin/godot.windows.editor.x86_64
PLATFORM=linuxbsd
AR=ar rcs
GODOT_TARGET_MODE=template_release
GODOT_DEBUG=0

# Windows
ifdef WINDOWS
ifndef _WIN32_WINNT
# _D_WIN32_WINNT must be greater than 0x0600 for sockets (inet_pton)
_WIN32_WINNT=0x0800
endif # _D_WIN32_WINNT
CC=x86_64-w64-mingw32-g++
CC_FLAGS=-D_WIN32_WINNT=$(_WIN32_WINNT) -O3 -pipe -static-libstdc++ -static-libgcc -fpermissive -Wall -Wno-reorder
CC_DEBUG_FLAGS=-D_WIN32_WINNT=$(_WIN32_WINNT) -O0 -g -ggdb -pipe -static-libstdc++ -static-libgcc -fpermissive -Wall -Wno-reorder
CC_LIBRARIES+=-lws2_32 -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive
TARGET_GODOT=externals/godot/bin/godot.windows.editor.x86_64.console.exe externals/godot/bin/godot.windows.editor.x86_64.exe
CLIENT_TERMINAL_BINAIRIES=
TARGET_CLIENT_TERMINAL=
PLATFORM=windows
BIN=$(BIN_WIN)
TARGET_SERVER=$(TARGET_SERVER_WIN)
endif # WINDOWS

# Debug
ifdef DEBUG
CC_FLAGS=$(CC_DEBUG_FLAGS)
GODOT_TARGET_MODE=template_debug
GODOT_DEBUG=1
LOG=true
else
endif # DEBUG

# Log
ifdef LOG
CC_FLAGS+=-DLOG
endif

# Directories

INCLUDE_AI=$(INCLUDE)/ai
INCLUDE_COM=$(INCLUDE)/communication
INCLUDE_DATA=$(INCLUDE)/data
INCLUDE_ACTIONS=$(INCLUDE_DATA)/actions
INCLUDE_ITEMS=$(INCLUDE_DATA)/items
INCLUDE_FURNITURES=$(INCLUDE_DATA)/furnitures
INCLUDE_SKILLS=$(INCLUDE_DATA)/skills
INCLUDE_WAYS=$(INCLUDE_DATA)/ways
INCLUDE_UTIL=$(INCLUDE)/util

INCLUDE_SERVER=$(INCLUDE)/server
INCLUDE_CLIENTS=$(INCLUDE)/clients
INCLUDE_CLIENT_TERMINAL=$(INCLUDE_CLIENTS)/terminal

SRC_AI=$(SRC)/ai
SRC_COM=$(SRC)/communication
SRC_DATA=$(SRC)/data
SRC_ACTIONS=$(SRC_DATA)/actions
SRC_ITEMS=$(SRC_DATA)/items
SRC_FURNITURES=$(SRC_DATA)/furnitures
SRC_SKILLS=$(SRC_DATA)/skills
SRC_WAYS=$(SRC_DATA)/ways
SRC_UTIL=$(SRC)/util

SRC_SERVER=$(SRC)/server
SRC_CLIENTS=$(SRC)/clients
SRC_CLIENT_TERMINAL=$(SRC_CLIENTS)/terminal

BIN_AI=$(BIN)/ai
BIN_COM=$(BIN)/communication
BIN_DATA=$(BIN)/data
BIN_ACTIONS=$(BIN_DATA)/actions
BIN_ITEMS=$(BIN_DATA)/items
BIN_FURNITURES=$(BIN_DATA)/furnitures
BIN_SKILLS=$(BIN_DATA)/skills
BIN_WAYS=$(BIN_DATA)/ways
BIN_UTIL=$(BIN)/util

BIN_SERVER=$(BIN)/server
BIN_CLIENTS=$(BIN)/clients
BIN_CLIENT_TERMINAL=$(BIN_CLIENTS)/terminal

# Files

AI_HEADERS=$(wildcard $(INCLUDE_AI)/*.h)
AI_SOURCES=$(patsubst $(INCLUDE_AI)/%.h,$(SRC_AI)/%.cpp,$(AI_HEADERS))
AI_BINAIRIES=$(patsubst $(INCLUDE_AI)/%.h,$(BIN_AI)/%.o,$(AI_HEADERS))

COM_HEADERS=$(wildcard $(INCLUDE_COM)/*.h)
COM_SOURCES=$(patsubst $(INCLUDE_COM)/%.h,$(SRC_COM)/%.cpp,$(COM_HEADERS))
COM_BINAIRIES=$(patsubst $(INCLUDE_COM)/%.h,$(BIN_COM)/%.o,$(COM_HEADERS))

DATA_HEADERS=$(wildcard $(INCLUDE_DATA)/*.h)
DATA_SOURCES=$(patsubst $(INCLUDE_DATA)/%.h,$(SRC_DATA)/%.cpp,$(DATA_HEADERS))
DATA_BINAIRIES=$(patsubst $(INCLUDE_DATA)/%.h,$(BIN_DATA)/%.o,$(DATA_HEADERS))

ACTIONS_HEADERS=$(wildcard $(INCLUDE_ACTIONS)/*.h)
ACTIONS_SOURCES=$(patsubst $(INCLUDE_ACTIONS)/%.h,$(SRC_ACTIONS)/%.cpp,$(ACTIONS_HEADERS))
ACTIONS_BINAIRIES=$(patsubst $(INCLUDE_ACTIONS)/%.h,$(BIN_ACTIONS)/%.o,$(ACTIONS_HEADERS))

ITEMS_HEADERS=$(wildcard $(INCLUDE_ITEMS)/*.h)
ITEMS_SOURCES=$(patsubst $(INCLUDE_ITEMS)/%.h,$(SRC_ITEMS)/%.cpp,$(ITEMS_HEADERS))
ITEMS_BINAIRIES=$(patsubst $(INCLUDE_ITEMS)/%.h,$(BIN_ITEMS)/%.o,$(ITEMS_HEADERS))

FURNITURES_HEADERS=$(wildcard $(INCLUDE_FURNITURES)/*.h)
FURNITURES_SOURCES=$(patsubst $(INCLUDE_FURNITURES)/%.h,$(SRC_FURNITURES)/%.cpp,$(FURNITURES_HEADERS))
FURNITURES_BINAIRIES=$(patsubst $(INCLUDE_FURNITURES)/%.h,$(BIN_FURNITURES)/%.o,$(FURNITURES_HEADERS))

SKILLS_HEADERS=$(wildcard $(INCLUDE_SKILLS)/*.h)
SKILLS_SOURCES=$(patsubst $(INCLUDE_SKILLS)/%.h,$(SRC_SKILLS)/%.cpp,$(SKILLS_HEADERS))
SKILLS_BINAIRIES=$(patsubst $(INCLUDE_SKILLS)/%.h,$(BIN_SKILLS)/%.o,$(SKILLS_HEADERS))

WAYS_HEADERS=$(wildcard $(INCLUDE_WAYS)/*.h)
WAYS_SOURCES=$(patsubst $(INCLUDE_WAYS)/%.h,$(SRC_WAYS)/%.cpp,$(WAYS_HEADERS))
WAYS_BINAIRIES=$(patsubst $(INCLUDE_WAYS)/%.h,$(BIN_WAYS)/%.o,$(WAYS_HEADERS))

UTIL_HEADERS=$(wildcard $(INCLUDE_UTIL)/*.h)
UTIL_SOURCES=$(patsubst $(INCLUDE_UTIL)/%.h,$(SRC_UTIL)/%.cpp,$(UTIL_HEADERS))
UTIL_BINAIRIES=$(patsubst $(INCLUDE_UTIL)/%.h,$(BIN_UTIL)/%.o,$(UTIL_HEADERS))

SERVER_HEADERS=$(wildcard $(INCLUDE_SERVER)/*.h)
SERVER_SOURCES=$(patsubst $(INCLUDE_SERVER)/%.h,$(SRC_SERVER)/%.cpp,$(SERVER_HEADERS))
SERVER_BINAIRIES=$(patsubst $(INCLUDE_SERVER)/%.h,$(BIN_SERVER)/%.o,$(SERVER_HEADERS))

CLIENTS_HEADERS=$(wildcard $(INCLUDE_CLIENTS)/*.h)
CLIENTS_SOURCES=$(patsubst $(INCLUDE_CLIENTS)/%.h,$(SRC_CLIENTS)/%.cpp,$(CLIENTS_HEADERS))
CLIENTS_BINAIRIES=$(patsubst $(INCLUDE_CLIENTS)/%.h,$(BIN_CLIENTS)/%.o,$(CLIENTS_HEADERS))

#CLIENT_TERMINAL_HEADERS=$(wildcard $(INCLUDE_CLIENT_TERMINAL)/*.h)
#CLIENT_TERMINAL_SOURCES=$(patsubst $(INCLUDE_CLIENT_TERMINAL)/%.h,$(SRC_CLIENT_TERMINAL)/%.cpp,$(CLIENT_TERMINAL_HEADERS))
#CLIENT_TERMINAL_BINAIRIES=$(patsubst $(INCLUDE_CLIENT_TERMINAL)/%.h,$(BIN_CLIENT_TERMINAL)/%.o,$(CLIENT_TERMINAL_HEADERS))

# Rules

ifndef WINDOWS
all: $(BIN) $(AI_BINAIRIES) $(DATA_BINAIRIES) $(COM_BINAIRIES) $(ACTIONS_BINAIRIES) $(ITEMS_BINAIRIES) $(FURNITURES_BINAIRIES) $(SKILLS_BINAIRIES) $(WAYS_BINAIRIES) $(UTIL_BINAIRIES) $(SERVER_BINAIRIES) $(CLIENTS_BINAIRIES) \
	$(TARGET_SERVER) $(TARGET_TEST) $(CLIENT_TERMINAL_BINAIRIES) $(TARGET_CLIENT_TERMINAL) $(BIN)/libsparkholders.a godot
else
all: $(BIN) $(AI_BINAIRIES) $(DATA_BINAIRIES) $(COM_BINAIRIES) $(ACTIONS_BINAIRIES) $(ITEMS_BINAIRIES) $(FURNITURES_BINAIRIES) $(SKILLS_BINAIRIES) $(WAYS_BINAIRIES) $(UTIL_BINAIRIES) $(SERVER_BINAIRIES) $(CLIENTS_BINAIRIES) \
	$(TARGET_SERVER) $(BIN)/libsparkholders.a godot
endif

$(BIN):
	$(MKDIR) $(BIN) $(BIN_AI) $(BIN_COM) $(BIN_DATA) $(BIN_ACTIONS) $(BIN_ITEMS) $(BIN_FURNITURES) $(BIN_SKILLS) $(BIN_WAYS) $(BIN_UTIL) $(BIN_SERVER) $(BIN_CLIENTS) $(BIN_CLIENT_TERMINAL)

$(BIN)/%.a: $(AI_BINAIRIES) $(COM_BINAIRIES) $(DATA_BINAIRIES) $(ACTIONS_BINAIRIES) $(ITEMS_BINAIRIES) $(FURNITURES_BINAIRIES) $(SKILLS_BINAIRIES) $(WAYS_BINAIRIES) $(UTIL_BINAIRIES) $(CLIENTS_BINAIRIES)
	$(AR) $@ $^

godot:
	cd externals/godot;	scons custom_modules=../../src/clients/graphics/godot_modules platform=$(PLATFORM) debug=$(GODOT_DEBUG)
	cd externals/godot;	scons custom_modules=../../src/clients/graphics/godot_modules platform=$(PLATFORM) target=$(GODOT_TARGET_MODE) arch=x86_64

$(BIN)/clients/terminal/%.o: $(SRC)/clients/terminal/%.cpp
	$(CC) $(CC_FLAGS) $(CC_INCLUDES) -c $< -o $@ $(CC_LIBRARIES) $(NCURSES_LIBRARIES)

$(BIN)/%.o: $(SRC)/%.cpp
	$(CC) $(CC_FLAGS) $(CC_INCLUDES) -c $< -o $@ $(CC_LIBRARIES)

$(TARGET_SERVER): $(AI_BINAIRIES) $(COM_BINAIRIES) $(DATA_BINAIRIES) $(ACTIONS_BINAIRIES) $(ITEMS_BINAIRIES) $(FURNITURES_BINAIRIES) $(SKILLS_BINAIRIES) $(WAYS_BINAIRIES) $(UTIL_BINAIRIES) $(SERVER_BINAIRIES)
	$(CC) $(CC_FLAGS) $(CC_INCLUDES) $^ -o $@ $(CC_LIBRARIES)

$(TARGET_TEST): $(AI_BINAIRIES) $(COM_BINAIRIES) $(DATA_BINAIRIES) $(ACTIONS_BINAIRIES) $(ITEMS_BINAIRIES) $(FURNITURES_BINAIRIES) $(SKILLS_BINAIRIES) $(WAYS_BINAIRIES) $(UTIL_BINAIRIES) tests/TestLauncher.cpp
	$(CC) $(CC_FLAGS) $(CC_INCLUDES) $^ -o $@ $(CC_LIBRARIES)

$(TARGET_CLIENT_TERMINAL): $(AI_BINAIRIES) $(COM_BINAIRIES) $(DATA_BINAIRIES) $(ACTIONS_BINAIRIES) $(ITEMS_BINAIRIES) $(FURNITURES_BINAIRIES) $(SKILLS_BINAIRIES) $(WAYS_BINAIRIES) $(UTIL_BINAIRIES) $(CLIENTS_BINAIRIES) $(CLIENT_TERMINAL_BINAIRIES)
	$(CC) $(CC_FLAGS) $(CC_INCLUDES) $^ -o $@ $(CC_LIBRARIES) $(NCURSES_LIBRARIES)

clean:
	$(RM) $(BIN_UNIX) $(BIN_WIN) $(TARGET_SERVER_UNIX) $(TARGET_SERVER_WIN) $(TARGET_TEST) $(TARGET_CLIENT_TERMINAL)
