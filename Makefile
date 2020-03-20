#########################[  SCRIPT CONSTANTS ]##################################

# - Colors:
WHITE     := \e[1;37m
BLACK     := \e[0;30m
GRAY      := \e[0;37m
BGRAY     := \e[1;30m
BLUE      := \e[0;34m
BBLUE     := \e[1;34m
GREEN     := \e[0;32m
BGREEN    := \e[1;32m
CYAN      := \e[0;36m
BCYAN     := \e[1;36m
RED       := \e[0;31m
BRED      := \e[1;31m
MAGENTA   := \e[0;35m
BMAGENTA  := \e[1;35m
YELLOW    := \e[0;33m
BYELLOW   := \e[1;33m
RESET     := \e[0m

# - Commands
FATAL     := @echo -e "$(BRED)[!!!]$(RESET)"
ERROR     := @echo -e "$(RED)[!!]$(RESET)"
WARNING   := @echo -e "$(BYELLOW)[!]$(RESET)"
NOTICE    := @echo -e "$(BGREEN)[*]$(RESET)"
INFO      := @echo -e "$(BCYAN)[+]$(RESET)"
DEBUG     := @echo -e "$(BGRAY)[\#]$(RESET)"

#########################[  BUILD VARIABLES ]###################################

# Environment Variables
PROJECT_NAME ?= ezi
PREFIX	     ?= /usr/local/bin/
BUILD_TYPE   ?= Debug
SRC_DIR      ?= src
INCLUDE_DIR  ?= include
BIN_DIR      ?= bin
BUILD_DIR    ?= build
SOURCE_EXT   ?= c
OBJECT_EXT   ?= o

# Compiler Variables
CC           := gcc
CC_FLAGS     := -c -I$(INCLUDE_DIR)
LINK_FLAGS   :=

# Validate build type, and set the compilation flags
ifeq ($(BUILD_TYPE), Release)
	CC_FLAGS += -O3
else ifeq ($(BUILD_TYPE), Debug)
	CC_FLAGS += -g
else
build_type_error:
	$(FATAL) "Invalid Build Type \`$(BUILD_TYPE)', Expected (Debug, Release)!";
	@exit 1;
endif

# Project Files
SOURCE_FILES := $(wildcard $(SRC_DIR)/*.$(SOURCE_EXT) \
				$(SRC_DIR)/*/*.$(SOURCE_EXT))
OBJECT_FILES := $(patsubst $(SRC_DIR)/%.$(SOURCE_EXT),\
				$(BUILD_DIR)/%.$(OBJECT_EXT), $(SOURCE_FILES))
TARGET_BIN   := $(BIN_DIR)/$(PROJECT_NAME)

###############################[ FUNCTIONS ]####################################

# Creates a directory
# - Params:
#   1 - The directory name
define MKDIR = 
	$(NOTICE) "Creating \`$(CYAN)$(1)/$(RESET)' Directory";
	@mkdir -p $(1)
endef

# Removes a directory and its contents
# - Params:
#   1 - The directory name
define RMDIR = 
	$(NOTICE) "Removing \`$(CYAN)$(1)/$(RESET)' Directory";
	@rm -rf $(1)
endef

############################[  BUILD RULES ]####################################

# A rule to produce the final binary
$(TARGET_BIN): $(OBJECT_FILES) $(BIN_DIR)
	$(INFO) "Linking to \`$(BCYAN)$@$(RESET)'";
	@$(CC) $(LINK_FLAGS) $< -o $@;
	$(NOTICE) "Done"

run: $(TARGET_BIN)
	$(DEBUG) "Executing \`$(BCYAN)$<$(RESET)<'\n";
	@./$<

# A rule to compile individual source files
$(BUILD_DIR)/%.$(OBJECT_EXT): $(SRC_DIR)/%.$(SOURCE_EXT) $(BUILD_DIR)
	$(INFO) "Compiling source \`$(CYAN)$<$(RESET)' to \`$(MAGENTA)$@$(RESET)'";
	@$(CC) $(CC_FLAGS) -o $@ $<;

# A rule to create the build directory
$(BUILD_DIR):
	$(call MKDIR,$(BUILD_DIR));

# A rule to create the output binaries directory
$(BIN_DIR):
	$(call MKDIR,$(BIN_DIR));

# A rule to clean up build files
clean:
	$(call RMDIR,$(BUILD_DIR));
	$(call RMDIR,$(BIN_DIR));

.PHONY: $(BIN_DIR)
.PHONY: $(BUILD_DIR)
.PHONY: clean
