CC = gcc


PROJECT_DIR := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))

     INCLUDE_DIR := $(PROJECT_DIR)include
      SOURCE_DIR := $(PROJECT_DIR)src
      OUTPUT_DIR := $(PROJECT_DIR)output
       TESTS_DIR := $(SOURCE_DIR)/tests
    EXAMPLES_DIR := $(SOURCE_DIR)/examples
   OUT_TESTS_DIR := $(OUTPUT_DIR)/tests
OUT_EXAMPLES_DIR := $(OUTPUT_DIR)/examples

INCLUDE_FILES := $(wildcard $(INCLUDE_DIR)/*.h)
   TEST_FILES := $(wildcard $(TESTS_DIR)/*.c)
EXAMPLE_FILES := $(wildcard $(EXAMPLES_DIR)/*.c)
    TEST_EXES := $(patsubst $(TESTS_DIR)/%.c,$(OUT_TESTS_DIR)/%.out,$(TEST_FILES))
 EXAMPLE_EXES := $(patsubst $(EXAMPLES_DIR)/%.c,$(OUT_EXAMPLES_DIR)/%.out,$(EXAMPLE_FILES))

# We do not care about optimizations...
CFLAGS := -g -O0 -std=gnu11 -I$(INCLUDE_DIR)

$(OUTPUT_DIR)/%.out: $(SOURCE_DIR)/%.c $(INCLUDE_FILES)
	$(CC) $(CFLAGS) -o $@ $<

prebuild:
	if [ ! -d "$(OUTPUT_DIR)" ]; then \
		mkdir -p $(OUTPUT_DIR); mkdir -p $(OUT_TESTS_DIR); mkdir -p $(OUT_EXAMPLES_DIR); \
	else \
		if [ ! -d "$(OUT_TESTS_DIR)" ]; then mkdir -p $(OUT_TESTS_DIR); fi; \
		if [ ! -d "$(OUT_EXAMPLES_DIR)" ]; then mkdir -p $(OUT_EXAMPLES_DIR); fi; \
	fi

clean:
	rm -f $(OUTPUT_DIR)/*.out
	rm -f $(OUT_TESTS_DIR)/*.out
	rm -f $(OUT_EXAMPLES_DIR)/*.out
	rm -rf $(OUT_TESTS_DIR)
	rm -rf $(OUT_EXAMPLES_DIR)
	rm -rf $(OUTPUT_DIR)

build-test: prebuild $(TEST_EXES)
build-example: prebuild $(EXAMPLE_EXES)

execute-test:
	for FILE in $(TEST_EXES); do $$FILE; done
execute-example:
	for FILE in $(EXAMPLE_EXES); do $$FILE; done

test: build-test execute-test
example: build-example execute-example