################################################################################
#                              Linux build Rules                               #
################################################################################

# Define object files
OBJS_FILES = $(subst $(SRC_DIR)/,$(BUILD_DIR)/,$(SRC_FILES))
OBJS := $(addsuffix .$(OBJ_EXT), $(OBJS_FILES))

# Set test objects
OBJ_TEST_FILES = $(subst $(SRC_DIR)/,$(BUILD_DIR)/,$(SRC_TEST_FILES))
OBJS_TEST := $(addsuffix .$(OBJ_EXT), $(OBJ_TEST_FILES))

# Define dependencies files
DEPS = $(OBJS:.$(OBJ_EXT)=.$(DEP_EXT)) $(OBJS_TEST:.$(OBJ_EXT)=.$(DEP_EXT))

# Add includes and macros to compiler options
CXXFLAGS += $(addprefix -I, $(INCLUDES))
CXXFLAGS += $(addprefix -D, $(MACROS))
DYNAMIC_LIBRARIES += dl elrond-runtime elrond

# LD settings
LD_RPATH = $(ldrp)
LDLIBS := -Wl,-rpath,\$$ORIGIN$(LD_RPATH) -L$(BUILD_DIR) $(LDLIBS)
LDLIBS += $(addprefix -l, $(DYNAMIC_LIBRARIES))

################################## BUILD RULES #################################

$(RUNTIME_CLI_PROG_NAME): $(BUILD_DIR)/$(RUNTIME_CLI_PROG_NAME)
$(BUILD_DIR)/$(RUNTIME_CLI_PROG_NAME): $(OBJS) $(BUILD_DIR)/$(RUNTIME_LIB_DYNAMIC_NAME)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

# Objects builder
$(BUILD_DIR)/%.$(CPP_SRC_EXT).$(OBJ_EXT): $(SRC_DIR)/%.$(CPP_SRC_EXT)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $< -o $@

# Runtime Base libary
$(BUILD_DIR)/$(RUNTIME_LIB_DYNAMIC_NAME): $(RUNTIME_LIB_SHARED)
	cp $^ $(RUNTIME_LIB_BUILD_DIR)/$(ELROND_SHARED_LIB_NAME) $(BUILD_DIR)

$(RUNTIME_LIB_SHARED):
	@cd $(RUNTIME_LIB_DIR) && $(MAKE) $(notdir $@)
