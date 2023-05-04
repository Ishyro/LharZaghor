# Utilities

MKDIR=mkdir -p
RM=rm -rf

# Directories

BIN=bin
EXEC=exec
TEST=test
SRC=src
INCLUDE=include
SCRIPTS=scripts

INCLUDE_AI=$(INCLUDE)/ai
INCLUDE_COM=$(INCLUDE)/communication
INCLUDE_DATA=$(INCLUDE)/data
INCLUDE_ACTIONS=$(INCLUDE_DATA)/actions
INCLUDE_SKILLS=$(INCLUDE_DATA)/skills
INCLUDE_UTIL=$(INCLUDE)/util

INCLUDE_SERVER=$(INCLUDE)/server
INCLUDE_CLIENTS=$(INCLUDE)/clients
INCLUDE_CLIENT_TERMINAL=$(INCLUDE_CLIENTS)/terminal
INCLUDE_CLIENT_GRAPHICS=$(INCLUDE_CLIENTS)/graphics

SRC_AI=$(SRC)/ai
SRC_COM=$(SRC)/communication
SRC_DATA=$(SRC)/data
SRC_ACTIONS=$(SRC_DATA)/actions
SRC_SKILLS=$(SRC_DATA)/skills
SRC_UTIL=$(SRC)/util

SRC_SERVER=$(SRC)/server
SRC_CLIENTS=$(SRC)/clients
SRC_CLIENT_TERMINAL=$(SRC_CLIENTS)/terminal
SRC_CLIENT_GRAPHICS=$(SRC_CLIENTS)/graphics

BIN_AI=$(BIN)/ai
BIN_COM=$(BIN)/communication
BIN_DATA=$(BIN)/data
BIN_ACTIONS=$(BIN_DATA)/actions
BIN_SKILLS=$(BIN_DATA)/skills
BIN_UTIL=$(BIN)/util

BIN_SERVER=$(BIN)/server
BIN_CLIENTS=$(BIN)/clients
BIN_CLIENT_TERMINAL=$(BIN_CLIENTS)/terminal
BIN_CLIENT_GRAPHICS=$(BIN_CLIENTS)/graphics

TARGET_SERVER=$(EXEC)/Server
TARGET_CLIENT_TERMINAL=$(EXEC)/Client_terminal
TARGET_CLIENT_GRAPHICS=$(EXEC)/Client_graphics

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

SKILLS_HEADERS=$(wildcard $(INCLUDE_SKILLS)/*.h)
SKILLS_SOURCES=$(patsubst $(INCLUDE_SKILLS)/%.h,$(SRC_SKILLS)/%.cpp,$(SKILLS_HEADERS))
SKILLS_BINAIRIES=$(patsubst $(INCLUDE_SKILLS)/%.h,$(BIN_SKILLS)/%.o,$(SKILLS_HEADERS))

UTIL_HEADERS=$(wildcard $(INCLUDE_UTIL)/*.h)
UTIL_SOURCES=$(patsubst $(INCLUDE_UTIL)/%.h,$(SRC_UTIL)/%.cpp,$(UTIL_HEADERS))
UTIL_BINAIRIES=$(patsubst $(INCLUDE_UTIL)/%.h,$(BIN_UTIL)/%.o,$(UTIL_HEADERS))

SERVER_HEADERS=$(wildcard $(INCLUDE_SERVER)/*.h)
SERVER_SOURCES=$(patsubst $(INCLUDE_SERVER)/%.h,$(SRC_SERVER)/%.cpp,$(SERVER_HEADERS))
SERVER_BINAIRIES=$(patsubst $(INCLUDE_SERVER)/%.h,$(BIN_SERVER)/%.o,$(SERVER_HEADERS))

CLIENTS_HEADERS=$(wildcard $(INCLUDE_CLIENTS)/*.h)
CLIENTS_SOURCES=$(patsubst $(INCLUDE_CLIENTS)/%.h,$(SRC_CLIENTS)/%.cpp,$(CLIENTS_HEADERS))
CLIENTS_BINAIRIES=$(patsubst $(INCLUDE_CLIENTS)/%.h,$(BIN_CLIENTS)/%.o,$(CLIENTS_HEADERS))

CLIENT_TERMINAL_HEADERS=$(wildcard $(INCLUDE_CLIENT_TERMINAL)/*.h)
CLIENT_TERMINAL_SOURCES=$(patsubst $(INCLUDE_CLIENT_TERMINAL)/%.h,$(SRC_CLIENT_TERMINAL)/%.cpp,$(CLIENT_TERMINAL_HEADERS))
CLIENT_TERMINAL_BINAIRIES=$(patsubst $(INCLUDE_CLIENT_TERMINAL)/%.h,$(BIN_CLIENT_TERMINAL)/%.o,$(CLIENT_TERMINAL_HEADERS))

CLIENT_GRAPHICS_HEADERS=$(wildcard $(INCLUDE_CLIENT_GRAPHICS)/*.h)
CLIENT_GRAPHICS_SOURCES=$(patsubst $(INCLUDE_CLIENT_GRAPHICS)/%.h,$(SRC_CLIENT_GRAPHICS)/%.cpp,$(CLIENT_GRAPHICS_HEADERS))
CLIENT_GRAPHICS_BINAIRIES=$(patsubst $(INCLUDE_CLIENT_GRAPHICS)/%.h,$(BIN_CLIENT_GRAPHICS)/%.o,$(CLIENT_GRAPHICS_HEADERS))

# Compiler and flags

CC=g++
CC_FLAGS=-O0 -pipe -pthread -fpermissive -g -Wall -Wno-reorder
CC_INCLUDES=-I $(INCLUDE)
CC_LIBRARIES=
NCURSES_LIBRARIES=-lncursesw -lformw -lmenuw -lpanelw
GRAPHICS_LIBRARIES=

# Windows
ifdef WINDOWS
ifndef _WIN32_WINNT
# _D_WIN32_WINNT must be greater than 0x0600 for sockets (inet_pton)
_WIN32_WINNT=0x0800
endif # _D_WIN32_WINNT
CC=x86_64-w64-mingw32-g++
CC_FLAGS=-D_WIN32_WINNT=$(_WIN32_WINNT) -O0 -pipe -static-libstdc++ -static-libgcc -fpermissive -g -Wall -Wno-reorder
CC_LIBRARIES+=-lws2_32
# We don't build the terminal client for Windows
CLIENTS_TERMINAL_BINAIRIES=
TARGET_CLIENT_TERMINAL=
endif # WINDOWS

# Rules

all: bin $(AI_BINAIRIES) $(DATA_BINAIRIES) $(COM_BINAIRIES) $(ACTIONS_BINAIRIES) $(SKILLS_BINAIRIES) $(UTIL_BINAIRIES) $(SERVER_BINAIRIES) $(CLIENTS_BINAIRIES) \
		exec $(TARGET_SERVER) $(CLIENTS_TERMINAL_BINAIRIES) $(TARGET_CLIENT_TERMINAL) $(CLIENTS_GRAPHICS_BINAIRIES) $(TARGET_CLIENT_GRAPHICS)

bin:
	$(MKDIR) $(BIN) $(BIN_AI) $(BIN_COM) $(BIN_DATA) $(BIN_ACTIONS) $(BIN_SKILLS) $(BIN_UTIL) $(BIN_SERVER) $(BIN_CLIENTS) $(BIN_CLIENT_GRAPHICS) $(BIN_CLIENT_TERMINAL)

exec:
	$(MKDIR) $(EXEC)

$(BIN)/clients/terminal/%.o : $(SRC)/clients/terminal/%.cpp
	$(CC) $(CC_FLAGS) $(CC_INCLUDES) -c $< -o $@ $(CC_LIBRARIES) $(NCURSES_LIBRARIES)

$(BIN)/%.o: $(SRC)/%.cpp
	$(CC) $(CC_FLAGS) $(CC_INCLUDES) -c $< -o $@ $(CC_LIBRARIES)

$(TARGET_SERVER): $(AI_BINAIRIES) $(COM_BINAIRIES) $(DATA_BINAIRIES) $(ACTIONS_BINAIRIES) $(SKILLS_BINAIRIES) $(UTIL_BINAIRIES) $(SERVER_BINAIRIES)
	$(CC) $(CC_FLAGS) $(CC_INCLUDES) $^ -o $@ $(CC_LIBRARIES)

$(TARGET_CLIENT_TERMINAL): $(AI_BINAIRIES) $(COM_BINAIRIES) $(DATA_BINAIRIES) $(ACTIONS_BINAIRIES) $(SKILLS_BINAIRIES) $(UTIL_BINAIRIES) $(CLIENTS_BINAIRIES) $(CLIENT_TERMINAL_BINAIRIES)
	$(CC) $(CC_FLAGS) $(CC_INCLUDES) $^ -o $@ $(CC_LIBRARIES) $(NCURSES_LIBRARIES)

$(TARGET_CLIENT_GRAPHICS): $(AI_BINAIRIES) $(COM_BINAIRIES) $(DATA_BINAIRIES) $(ACTIONS_BINAIRIES) $(SKILLS_BINAIRIES) $(UTIL_BINAIRIES) $(CLIENTS_BINAIRIES) $(CLIENT_GRAPHICS_BINAIRIES)
	$(CC) $(CC_FLAGS) $(CC_INCLUDES) $^ -o $@ $(CC_LIBRARIES) $(GRAPHICS_LIBRARIES)

clean:
	$(RM) $(BIN) $(EXEC)
